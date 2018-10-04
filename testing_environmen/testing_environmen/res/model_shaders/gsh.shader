#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 world_position[];
in vec3 normal[];
in vec2 otexture_coords[];
in vec4 pass_shadow[];

out vec3 pass_world_position;
out vec3 pass_normal;
out vec2 pass_texture_coords;
out vec4 shadow;

vec3 calculate_normal(int i1, int i2, int i3)
{
	vec3 diff_world_pos1 = normalize(world_position[i2] - world_position[i1]);
	vec3 diff_world_pos2 = normalize(world_position[i3] - world_position[i1]);
	return normalize(cross(diff_world_pos2, diff_world_pos1));
}

void main(void)
{
	for (int i = 0; i < 3; ++i)
	{
		gl_Position = gl_in[i].gl_Position;

		//pass_normal = normalize(normal[2]);
		//pass_normal = calculate_normal(i, (i + 1) % 3, (i - 2) % 3);
		pass_normal = calculate_normal(0, 2, 1);
		pass_normal.x *= -1;
		pass_normal.z *= -1;
		pass_world_position = world_position[i];
		shadow = pass_shadow[i];
		pass_texture_coords = otexture_coords[i];

		EmitVertex();
	}
	EndPrimitive();
}
