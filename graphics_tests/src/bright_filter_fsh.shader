#version 330 core

in vec2 pass_texture_coords;
out vec4 final_color;

uniform sampler2D colored_texture;

void main(void)
{
	vec4 color = texture(colored_texture, pass_texture_coords);
	// luma conversion
	float brightness = (color.r * 0.2126) + (color.g * 0.7152) + (color.b * 0.0722);

	final_color = color * brightness;
}