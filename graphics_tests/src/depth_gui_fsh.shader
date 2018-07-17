#version 330 core

in  vec2 pass_texture_coords;
out vec4 final_color;

uniform sampler2D diffuse;

vec4 get_depth_color(void)
{
	float d = texture(diffuse, pass_texture_coords).x;

//	if (abs(d - 1.0f) < 0.000001f) d = 0.0f;

	return vec4(d, d, d, 1.0f);
}

void main(void)
{
	final_color = get_depth_color();
}