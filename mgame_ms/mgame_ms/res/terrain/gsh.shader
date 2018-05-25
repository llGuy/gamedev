#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 pass_vertex_color[];
in vec3 pass_vertex_world_position[];

out vec3 color;
out vec3 vertex_normal;
out vec4 diffuse_light;

uniform vec3 light_position;

// calculate the normal of the triangle
vec3 Normal(void)
{
	vec3 diff_world_pos1 = normalize(pass_vertex_world_position[1] - pass_vertex_world_position[0]);
	vec3 diff_world_pos2 = normalize(pass_vertex_world_position[2] - pass_vertex_world_position[0]);
	return normalize(cross(diff_world_pos1, diff_world_pos2));
}

vec4 Diffuse(vec3 normal, vec3 light_vector)
{
	const float SCALE = 0.8f;
	float brightness = dot(light_vector, normal) * SCALE;
	return vec4(brightness, brightness, brightness, 1.0f);
}

void main(void)
{
	vec3 triangle_normal = Normal();
	vec4 d_light = Diffuse(triangle_normal, (light_position - pass_vertex_world_position[0]));

	for (int i = 0; i < 3; ++i)
	{
		gl_Position = gl_in[i].gl_Position;
		vertex_normal = triangle_normal;
		diffuse_light = d_light;
		color = pass_vertex_color[i];
		EmitVertex();
	}
	EndPrimitive();
}