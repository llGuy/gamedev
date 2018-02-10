#version 430

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

in vec3 pass_world_position[];
in vec3 pass_texture_data[];
in float pass_visibility[];

out vec3 normal;
out float texture_data;
out float visibility;
out vec2 texture_coords;
out vec3 vertex_position;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

// for the fog
const float DENSITY = 0.007f;
const float GRADIENT = 1.5f;

vec3 CalculateNormal(vec3 v[3])
{
	vec3 diffWorldPos1 = normalize(v[1] - v[0]);
	vec3 diffWorldPos2 = normalize(v[2] - v[0]);
	return normalize(cross(diffWorldPos1, diffWorldPos2));
}

struct FaceTextureMap
{
	vec2 tx00;
	vec2 tx01;
	vec2 tx10;
	vec2 tx11;
	// 0 = top, 1 = sides, 2 = bottom
	int f;
};

FaceTextureMap GenerateTextureMap(int index)
{
	float center = 0.0007f;
	FaceTextureMap ftm;
	float tdu = mod(pass_texture_data[0][index], 16) / 16.0f;
	float tdv = floor(pass_texture_data[0][index] / 16.0f) / 16.0f;
	ftm.tx00 = vec2(tdu + center, tdv + center);
	ftm.tx01 = vec2(tdu + 1.0f / 16.0f - center, tdv + center);
	ftm.tx10 = vec2(tdu + center, tdv + 1.0f / 16.0f - center);
	ftm.tx11 = vec2(tdu + 1.0f / 16.0f - center, tdv + 1.0f / 16.0f - center);
	ftm.f = index;
	return ftm;
}

FaceTextureMap GenerateFaceTextureMap(vec3 flags)
{
	if (abs(flags.x) > 0.1f || abs(flags.z) > 0.1f) return GenerateTextureMap(1);
	else if (flags.y > 0.1f) return GenerateTextureMap(0);
	else if (flags.y  < -0.1f) return GenerateTextureMap(2);
}

float CreateOutTextureData(int f)
{
	return pass_texture_data[0][f];
}

float CalculateFog(vec4 relativeToCameraPosition)
{
	float distance = length(relativeToCameraPosition.xyz);
	float vis = exp(-pow((distance * DENSITY), GRADIENT));
	vis = clamp(vis, 0.0, 1.0);
	return vis;
}

void CreateVertex(vec3 offset, vec3 n, vec2 tcoords, int f)
{
	// calculating world position
	vec3 actual_offset = offset * 0.5f;
	vec3 world_position = pass_world_position[0] + actual_offset;
	// position relative to the camera
	vec4 view_position = view_matrix * vec4(world_position, 1.0f);
	gl_Position = projection_matrix * view_position;
	// texture coordinates
	texture_coords = tcoords;
	// fog calculations
	vec4 relativeToCameraPosition = view_position;
	visibility = CalculateFog(relativeToCameraPosition);
	// which face the vertex belongs to
	texture_data = CreateOutTextureData(f);
	// normal
	normal = n;
	vertex_position = world_position;
	EmitVertex();
}

void CreateFaceVertex(vec3 flags, float vcomp1, float vcomp2, unsigned int i, vec2 tcoords, int f)
{
	vec3 v1;
	// creates vertex at the face that the flag is enabling
	v1[i] = flags[i];
	v1[(i + 1) % 3] = vcomp1;
	v1[(i + 2) % 3] = vcomp2;
	CreateVertex(v1, flags, tcoords, f);
}

void CreateFace(vec3 flags)
{
	for (unsigned int i = 0; i < 3; ++i)
	{
		if (abs(flags[i]) != 0)
		{
			FaceTextureMap ftm = GenerateFaceTextureMap(flags);
			if (i == 0)
			{
				CreateFaceVertex(flags, -1.0f, -1.0f, i, ftm.tx11, ftm.f);
				CreateFaceVertex(flags, 1.0f, -1.0f, i, ftm.tx01, ftm.f);
				CreateFaceVertex(flags, -1.0f, 1.0f, i, ftm.tx10, ftm.f);
				CreateFaceVertex(flags, 1.0f, 1.0f, i, ftm.tx00, ftm.f);
			}
			else
			{
				CreateFaceVertex(flags, -1.0f, -1.0f, i, ftm.tx11, ftm.f);
				CreateFaceVertex(flags, 1.0f, -1.0f, i, ftm.tx10, ftm.f);
				CreateFaceVertex(flags, -1.0f, 1.0f, i, ftm.tx01, ftm.f);
				CreateFaceVertex(flags, 1.0f, 1.0f, i, ftm.tx00, ftm.f);
			}
			EndPrimitive();
			return;
		}
	}
}

void CreateCube(void)
{
	CreateFace(vec3(1.0f, 0.0f, 0.0f));
	CreateFace(vec3(-1.0f, 0.0f, 0.0f));
	CreateFace(vec3(0.0f, 1.0f, 0.0f));
	CreateFace(vec3(0.0f, -1.0f, 0.0f));
	CreateFace(vec3(0.0f, 0.0f, 1.0f));
	CreateFace(vec3(0.0f, 0.0f, -1.0f));
}

void main()
{
	CreateCube();
}