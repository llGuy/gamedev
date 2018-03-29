#version 330 core

in vec2 pass_texture_coordinate;
out vec4 fragment_color;

uniform sampler2D diffuse_texture;

void main(void)
{
	fragment_color = texture2D(diffuse_texture, pass_texture_coordinate);
}