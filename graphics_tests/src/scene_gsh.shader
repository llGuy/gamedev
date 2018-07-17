#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 pass_world_positions[];
in vec2 pass_texture_coords[];
in vec4 pass_shadow_coord[];
out vec2 final_texture_coord;
out vec3 final_normal;
out vec3 final_position;
out vec3 color;
out vec4 shadow_coord;

uniform vec3 model_color;

vec3 get_normal(void)
{
	vec3 a = pass_world_positions[1] - pass_world_positions[0];
	vec3 b = pass_world_positions[2] - pass_world_positions[0];
	return normalize(cross(a, b));
}

void main(void)
{
	vec3 normal = get_normal();
	
	for (int i = 0; i < 3; ++i)
	{
		gl_Position = gl_in[i].gl_Position;
		final_texture_coord = pass_texture_coords[i];
		final_normal = normal;
		final_position = pass_world_positions[i];
		color = pass_world_positions[0] / 8.0f;
		shadow_coord = pass_shadow_coord[i];

		EmitVertex();
	}
	EndPrimitive();
}