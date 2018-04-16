#version 330 core

in vec3 pass_vertex_color;
out vec4 fragment_color;

void main()
{
	fragment_color = vec4(pass_vertex_color, 1.0f);
}