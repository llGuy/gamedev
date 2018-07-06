#version 330 core

out vec4 final_color;
in vec2 final_texture_coord;

uniform sampler2D background;
uniform sampler2D r_texture;
uniform sampler2D g_texture;
uniform sampler2D b_texture;
uniform sampler2D blend_map;

uniform vec3 model_color;

void main(void)
{
	vec4 blend_map_color = texture(blend_map, final_texture_coord);
	float back_texture_amount = 1 - (blend_map_color.r + blend_map_color.g + blend_map_color.b);
	vec2 tiled_coords = final_texture_coord * 1000.0f;

	vec4 background_texture_color = texture(background, tiled_coords) * back_texture_amount;

	vec4 r_texture_color = texture(r_texture, tiled_coords) * blend_map_color.r;
	vec4 g_texture_color = texture(g_texture, tiled_coords) * blend_map_color.g;
	vec4 b_texture_color = texture(b_texture, tiled_coords) * blend_map_color.b;

	vec4 total_color = background_texture_color + r_texture_color + g_texture_color + b_texture_color;

	final_color = total_color;
}