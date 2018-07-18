#version 330 core

in  vec2 pass_texture_coords;
out vec4 final_color;

uniform sampler2D diffuse;

//vec4 get_depth_color(void)
//{
//	float d = texture(diffuse, pass_texture_coords).x;
//
//	return vec4(d, d, d, 1.0f);
//}

void main(void)
{
	final_color = texture(diffuse, pass_texture_coords);
}