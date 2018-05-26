#version 320 es

precision highp float;

out vec4 final_color;

in vec3 pass_vertex_color;
in vec3 pass_vertex_world_position;

void main(void)
{
	final_color = vec4(pass_vertex_color, 1.0f);// + diffuse_light;
}