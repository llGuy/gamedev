#version 330 core

in vec3 color;

out vec4 final_fragment_color;

void main(void)
{
	final_fragment_color = vec4(color, 1.0f);
}