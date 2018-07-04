#version 330 core

out vec4 final_color;

uniform vec3 model_color;

void main(void)
{
	final_color = vec4(model_color, 1.0f);
}