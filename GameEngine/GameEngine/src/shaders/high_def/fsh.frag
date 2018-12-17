#version 400

layout(location = 0) out vec4 final_color;

uniform sampler2D diffuse;
uniform sampler2D shadow_map;
uniform vec3 camera_position;

layout(std140, row_major) uniform light
{
    vec4 light_position;
	vec4 ambient_intensity;
	vec4 diffuse_intensity;
	vec4 specular_intensity;
} 
light_info;

layout(std140) uniform shadow_data 
{
	mat4 shadow_bias;

	float transition_distance;
	float shadow_distance;
	float map_size;
	float pcf_count;
} 
shadow_info;

in VS_DATA
{
	vec4 shadow_coord;
	vec3 ws3D_position;
	vec2 uvs;
	vec3 normal;
}
data;

float get_shadow_value(in vec3 ws3D_position, in vec4 shadow_coord);
void apply_diffuse(inout vec4 dest, in vec3 light_vector, in vec3 nrm, in float light_factor);


void main(void)
{
	final_color = texture(diffuse, data.uvs);

	vec3 light_vector =	normalize(light_info.light_position.xyz);
	vec3 eye_vector	= normalize(camera_position	- data.ws3D_position);
	
	/* Calculate how much the pixel is affected by shadows */
	float light_factor = get_shadow_value(data.ws3D_position, data.shadow_coord);

	apply_diffuse(final_color, light_vector, data.normal, light_factor);
}


void apply_diffuse(inout vec4 dest, in vec3 light_vector, in vec3 nrm, in float light_factor)
{
	float diffuse = clamp(dot(light_vector, nrm), 0, 1);

	dest = vec4(diffuse) * light_info.diffuse_intensity * light_factor + dest;
}

float get_shadow_value(in vec3 ws3D_position, in vec4 shadow_coord)
{
	float total_texels = (shadow_info.pcf_count * 2.0f + 1.0f) * (shadow_info.pcf_count * 2.0f + 1.0f);

	float dist = distance(ws3D_position, camera_position);

	dist = dist - (shadow_info.shadow_distance - shadow_info.transition_distance);
	dist = dist / shadow_info.transition_distance;
	dist = clamp(1.0 - dist, 0.0, 1.0);

	float texel_size = 1.0f / shadow_info.map_size;
	float total = 0.0f;

	if (shadow_coord.x <= 1.0f && shadow_coord.y <= 1.0f && shadow_coord.z <= 1.0f)
	{
		for (int x = int(-shadow_info.pcf_count); x <= int(shadow_info.pcf_count); ++x)
		{
			for (int y = int(-shadow_info.pcf_count); y <= int(shadow_info.pcf_count); ++y)
			{
				float object_nearest_light = texture(shadow_map, shadow_coord.xy + vec2(x, y) * texel_size).x;
				if (shadow_coord.z > object_nearest_light + 0.002f)
				{
					total += 0.5f;
				}
			}
		}
		total /= total_texels;
	}

	float light_factor = 1.0f - (total * dist);

	return light_factor;
}