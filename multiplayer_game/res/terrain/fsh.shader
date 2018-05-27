#version 330

precision highp float;

out vec4 final_color;

in vec4 diffuse;
in vec3 vertex_color;

void main(void)
{
	final_color = vec4(vertex_color, 1.0f) + clamp(diffuse, -0.15, 1);
}