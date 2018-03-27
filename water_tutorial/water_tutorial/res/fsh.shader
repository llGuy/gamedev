#version 330 core

in vec3 pass_colour;
out vec4 fragment_colour;

void main(void)
{
	fragment_colour = vec4(pass_colour, 1.0f);
}