#version 330 core

in vec3 texture_coords;
out vec4 final_color;

uniform samplerCube cube_map;

void main(void)
{
	final_color = texture(cube_map, texture_coords);
}