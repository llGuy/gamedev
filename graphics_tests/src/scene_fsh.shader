
#version 330 core

out vec4 final_color;
in vec2 final_texture_coord;
in vec3 final_normal;
in vec3 final_position;
in vec3 color;
in vec4 shadow_coord;

uniform sampler2D background;
uniform sampler2D r_texture;
uniform sampler2D g_texture;
uniform sampler2D b_texture;
uniform sampler2D blend_map;
uniform sampler2D shadow_map;
uniform vec3 model_color;

const vec3 light_position = vec3(0.0f, 1000.0f, 0.0f);

vec4 get_diffuse(void)
{
	vec3 normal = vec3(0.0f, 1.0f, 0.0f);
	vec3 light_vector = normalize(light_position - final_position);
	float d = dot(light_vector, normal);
	return vec4(d, d, d, 1.0f) * 0.4f;
}

void main(void)
{
	vec4 blend_map_color = texture(blend_map, final_texture_coord);
	float back_texture_amount = 1 - (blend_map_color.r + blend_map_color.g + blend_map_color.b);
	vec2 tiled_coords = final_texture_coord * 20.0f;

	vec4 background_texture_color = texture(background, tiled_coords) * back_texture_amount;

	vec4 r_texture_color = texture(r_texture, tiled_coords) * blend_map_color.r;
	vec4 g_texture_color = texture(g_texture, tiled_coords) * blend_map_color.g;
	vec4 b_texture_color = texture(b_texture, tiled_coords) * blend_map_color.b;

	vec4 total_color = background_texture_color + r_texture_color + g_texture_color + b_texture_color;

	float visibility = 1.0f;
	if (texture(shadow_map, shadow_coord.xy).x < shadow_coord.z)
	{
		visibility = 0.5f;
	}

	final_color = visibility * (total_color + get_diffuse());
}