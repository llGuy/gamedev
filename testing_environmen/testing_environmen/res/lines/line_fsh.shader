#version 330 core

in vec4 pass_vertex_color;
out vec4 final_color;

void main(void) 
{
	final_color = pass_vertex_color;
}