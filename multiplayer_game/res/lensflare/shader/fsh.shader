#version 330

in vec2 pass_texture_coordinate;

out vec4 final_color;

uniform sampler2D diffuse_texture;
uniform float brightness;

void main(void)
{
	vec4 color = texture(diffuse_texture, pass_texture_coordinate) * brightness;
	//final_color = vec4(0.0f, 0.0f, 1.0f, 0.5f);
}