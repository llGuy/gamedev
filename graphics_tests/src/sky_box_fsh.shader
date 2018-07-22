#version 330 core

in vec3 texture_coords;
out vec4 final_color;

uniform samplerCube cube_map;

void main(void)
{
	//final_color = texture(cube_map, texture_coords / 500.0f);
	final_color = vec4(texture_coords, 1.0f);
//	final_color.xyz /= 500.0f;
	final_color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}