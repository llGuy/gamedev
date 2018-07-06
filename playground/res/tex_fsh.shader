#version 330 core

in vec2 pass_texture_coords;
out vec4 final_color;

uniform sampler2D diffuse;

void main(void)
{
	final_color = texture2D(diffuse, pass_texture_coords);
}