#version 330 core

in vec3 pass_color;
out vec4 final_color;

void main(void)
{
	final_color = vec4(pass_color, 1.0f);
}