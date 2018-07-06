#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 pass_texture_coords[];
out vec2 final_texture_coord;

uniform vec3 model_color;

void main(void)
{
	for (int i = 0; i < 3; ++i)
	{
		gl_Position = gl_in[i].gl_Position;
		final_texture_coord = pass_texture_coords[i];

		EmitVertex();
	}
	EndPrimitive();
}