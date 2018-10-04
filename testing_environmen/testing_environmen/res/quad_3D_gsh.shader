#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 pass_world_position[];
in vec4 shadow_coord[];

out vec3 world_pos;
out vec3 light_vector;
out vec3 normal;
out vec4 shadow;

uniform vec3 camera_pos;
uniform vec3 light_position;

vec3 calculate_normal(int i1, int i2, int i3)
{
	vec3 diff_world_pos1 = normalize(pass_world_position[i2] - pass_world_position[i1]);
	vec3 diff_world_pos2 = normalize(pass_world_position[i3] - pass_world_position[i1]);
	return normalize(cross(diff_world_pos2, diff_world_pos1));
}

void main(void)
{
	for (int i = 0; i < 3; ++i)
	{
		gl_Position = gl_in[i].gl_Position;

		//normal = calculate_normal(i, (i + 1) % 3, (i + 2) % 3);
		normal = calculate_normal(0, 1, 2);
		light_vector = normalize(light_position - pass_world_position[i]);
		shadow = shadow_coord[i];
		world_pos = pass_world_position[i];

		EmitVertex();
	}
	EndPrimitive();
}