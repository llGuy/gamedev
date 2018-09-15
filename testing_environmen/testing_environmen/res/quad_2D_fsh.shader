#version 330 core

in vec2 pass_texture_coords;
out vec4 final_color;

uniform sampler2D diffuse;
uniform vec3 color;

void main(void)
{
	final_color = texture(diffuse, pass_texture_coords);
}