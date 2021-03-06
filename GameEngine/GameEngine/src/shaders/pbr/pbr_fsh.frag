#version 400

layout(location = 0) out vec4 final_color;
layout(location = 1) out vec4 bright_color;
layout(location = 2) out vec4 view_position;
layout(location = 3) out vec4 view_normal;
layout(location = 4) out vec4 sun_only;

uniform sampler2D diffuse_map;
uniform sampler2D normal_map;
uniform sampler2D shadow_map;
uniform sampler2D disp_map;
uniform sampler2D metallic;
uniform sampler2D roughness;

in VS_OUT {
	vec3 position;
	vec2 uvs;
	vec3 normal;
	vec4 shadow_coord;
	vec3 tangent;
	vec3 bitangent;
	vec4 view_position;
    vec4 tangent_view_position;
	vec3 to_light;
	vec3 to_camera;
	vec3 tangent_camera_pos;

	vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;

	mat3 inv_tangent;
	mat3 to_tangent_space;
	mat3 ws_inv_tangent;
}
fs_in;

layout(std140) uniform shadow_data
{
	mat4 shadow_bias;

	float transition_distance;
	float shadow_distance;
	float map_size;
	float pcf_count;
} 
shadow_info;

layout(std140) uniform camera
{
	vec4 position;
	mat4 view_matrix;
	mat4 projection_matrix;
} 
camera_info;

float get_shadow_factor()
{
	vec3 world_pos = fs_in.position;
	vec4 shadow_coord = fs_in.shadow_coord;

	float total_texels = (shadow_info.pcf_count * 2.0 + 1.0) * (shadow_info.pcf_count * 2.0 + 1.0);

	float dist = distance(world_pos, vec3(camera_info.position));

	dist = dist - (shadow_info.shadow_distance - shadow_info.transition_distance);
	dist = dist / shadow_info.transition_distance;
	dist = clamp(1.0 - dist, 0.0, 1.0);

	float texel_size = 1.0 / shadow_info.map_size;
	float total = 0.0;

	if (shadow_coord.x <= 1.0 && shadow_coord.y <= 1.0 && shadow_coord.z <= 1.0)
	{
		for (int x = int(-shadow_info.pcf_count); x <= int(shadow_info.pcf_count); ++x)
		{
			for (int y = int(-shadow_info.pcf_count); y <= int(shadow_info.pcf_count); ++y)
			{
				float object_nearest_light = texture(shadow_map, shadow_coord.xy + vec2(x, y) * texel_size).x;
				if (shadow_coord.z > object_nearest_light + 0.002)
				{
					total += 0.6;
				}
			}
		}
		total /= total_texels;
	}

	float light_factor = 1.0 - (total * dist);

	return light_factor;
}

vec2 parallax()
{
	vec2 uvs = fs_in.uvs;
	vec3 view_dir = normalize(vec3(fs_in.tangent_camera_pos) - vec3(fs_in.tangent_view_position));
	view_dir.y *= -1;
	const float min_layers = 8.0;
	const float max_layers = 32.0;
	float num_layers = mix(max_layers, min_layers, abs(dot(vec3(0.0, 0.0, 1.0), view_dir)));

	const float height_scale = 0.1;
	float layer_depth = 1.0 / num_layers;
	float current_layer_depth = 0.0;

	vec2 P = view_dir.xy * height_scale;
	vec2 delta_uvs = P / num_layers;

	vec2 current_uv = fs_in.uvs;
	float current_depth_map_value = texture(disp_map, current_uv).r;

	while(current_layer_depth < current_depth_map_value)
	{
		current_uv -= delta_uvs;
		current_depth_map_value = texture(disp_map, current_uv).r;
		current_layer_depth += layer_depth;
	}

	vec2 prev_uv = current_uv + delta_uvs;

	float after_depth = current_depth_map_value - current_layer_depth;
	float before_depth = texture(disp_map, prev_uv).r - current_layer_depth + layer_depth;

	float weight = after_depth / (after_depth - before_depth);
	vec2 final_uv = prev_uv * weight + current_uv * (1.0 - weight);

	return final_uv;
}

bool is_1(in vec3 value)
{
	float prec = 0.2;
	vec3 diff = abs(value - vec3(1.0));

	return diff.x < prec && diff.y < prec && diff.z < prec;
}

void main(void)
{
	//vec2 displacement = parallax();
	vec2 displacement = fs_in.uvs;
	
	vec4 color = texture(diffuse_map, displacement);

	float metalness_val = texture(metallic, displacement).x;
	float roughness_val = texture(roughness, displacement).x;

	vec4 normal = 2.0 * texture(normal_map, displacement) - 1.0;

	vec3 unit_normal = normalize(normal.rbg);

	vec3 to_light = fs_in.to_light;
	 
	float shadow_factor = get_shadow_factor();

/*	float brightness = clamp(dot(normalize(unit_normal), normalize(to_light)), 0.0, 1.0) * 0.7;
   
	vec3 reflected_light = reflect(normalize(to_light), unit_normal);
	float specular = clamp(dot(reflected_light, normalize(fs_in.to_camera)), 0, 1);
	specular = pow(specular, 10) * 0.7;*/

	final_color = (color) * 1.0;

	final_color.a = roughness_val;

	//final_color = vec4(fs_in.inv_tangent * vec3(0, 1, 0), 1.0);

	//view_normal = vec4(fs_in.normal, 1.0);

	view_normal = camera_info.view_matrix * vec4(fs_in.ws_inv_tangent * normal.rgb, 0.0);
	view_normal.a = metalness_val;
//	view_normal.z *= -1;
//view_normal = vec4(normal.rbg, 1.0);

	view_position = fs_in.view_position;

	float bright_colorness = (final_color.r * 0.2126) + (final_color.g * 0.7152) + (final_color.b * 0.722);
	
	if (bright_colorness > 0.9)
	{
		bright_color = final_color;
	}
	else
	{
		bright_color = vec4(0);
	}

	sun_only = vec4(0.0, 0.0, 0.0, 1.0);
}