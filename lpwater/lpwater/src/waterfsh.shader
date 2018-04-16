#version 330 core

in vec3 pass_vertex_color;
in vec4 pass_projected_position;
out vec4 fragment_color;

void main(void)
{
	fragment_color = vec4(pass_vertex_color, 1.0f);
}