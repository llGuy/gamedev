#version 330 core

in vec3 pass_colour;
out vec4 fragment_colour;

//uniform sampler2D reflectionTexture;
//uniform sampler2D refractionTexture;

void main(void)
{
	fragment_colour = vec4(pass_colour, 1.0f);
}