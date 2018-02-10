#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 pass_worldPosition[];
in vec3 pass_colour[];

uniform vec3 u_lightPosition;
uniform vec3 u_eyePosition;

out vec3 color;
out vec4 diffuseLight;
out vec4 specularLight;

vec3 CalculateNormal(void)
{
	vec3 diffWorldPos1 = normalize(pass_worldPosition[1] - pass_worldPosition[0]);
	vec3 diffWorldPos2 = normalize(pass_worldPosition[2] - pass_worldPosition[0]);
	return normalize(cross(diffWorldPos1, diffWorldPos2));
}

vec4 CalculateDiffuse(vec3 normal, vec3 lightVector)
{
	float brightness = dot(lightVector, normalize(normal)) * 0.4f;
	return vec4(brightness, brightness, brightness, 1.0);
}

vec4 CalculateSpecularity(vec3 lightVector, vec3 normal, vec3 vertexPosition)
{
	vec3 reflectedLightWorld = reflect(-lightVector, normal);
	vec3 eyeVector = normalize(u_eyePosition - vertexPosition);
	float specularity = dot(reflectedLightWorld, eyeVector);
	return vec4(specularity, specularity, specularity, 1);
}

void main()
{
	vec3 lightVector = normalize(u_lightPosition - pass_worldPosition[0]);

	vec3 normal = CalculateNormal();
	vec4 d = CalculateDiffuse(normal, lightVector);
	vec4 s = CalculateSpecularity(lightVector, normal, pass_worldPosition[0]);

	for (int i = 0; i < 3; i++)
	{
		gl_Position = gl_in[i].gl_Position;

		color = pass_colour[0];

		diffuseLight = d;

		specularLight = s;

		EmitVertex();
	}
	EndPrimitive();
}