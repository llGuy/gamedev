#version 330 core

in vec2 pass_texture_coordinate;
out vec4 fragment_color;

uniform sampler2D reflection_texture;
uniform sampler2D refraction_texture;

void main(void)
{
	vec4 reflect_colour = texture2D(reflection_texture, pass_texture_coordinate);
	vec4 refract_colour = texture2D(refraction_texture, pass_texture_coordinate);

	fragment_color = mix(reflect_colour, refract_colour, 0.5f);
}