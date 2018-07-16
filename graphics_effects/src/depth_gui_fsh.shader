#version 330 core

in  vec2 pass_texture_coords;
out vec4 final_color;

uniform sampler2D diffuse;

vec4 get_depth_color(void)
{
	vec4 d = texture(diffuse, pass_texture_coords);
	return d;
}

void main(void)
{
	final_color = vec4(pass_texture_coords, 0.0f, 1.0f);
	//final_color = get_depth_color();
}