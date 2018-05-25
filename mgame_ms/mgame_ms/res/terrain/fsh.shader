#version 430

precision highp float;

out vec4 final_color;

in vec3 color;
in vec3 vertex_normal;
in vec4 diffuse_light;

void main(void)
{
	final_color = vec4(color, 1.0f);// + diffuse_light;
}