#version 330 core

out vec4 final_color;
in vec3 light_vector;
in vec3 normal;
in vec4 shadow;
in vec3 world_pos;
in float dist;

uniform vec3 light_position;
uniform vec3 camera_pos;
uniform sampler2D shadow_map;
uniform vec3 color;

const float map_size = 2048.0f * 2.0f;
const int pcf_count = 2;
const float total_texels = (pcf_count * 2.0f + 1.0f) * (pcf_count * 2.0f + 1.0f);

const float shadow_distance = 100.0f;
const float transition_distance = 20.0f;

void main(void)
{
	float dist = distance(world_pos, camera_pos);

	dist = dist - (shadow_distance - transition_distance);
	dist = dist / transition_distance;
	dist = clamp(1.0 - dist, 0.0, 1.0);

	float texel_size = 1.0f / map_size;
	float total = 0.0f;

	if (shadow.x <= 1.0f && shadow.y <= 1.0f && shadow.z <= 1.0f)
	{
		for (int x = -pcf_count; x <= pcf_count; ++x)
		{
			for (int y = -pcf_count; y <= pcf_count; ++y)
			{
				float object_nearest_light = texture(shadow_map, shadow.xy + vec2(x, y) * texel_size).x;
				if (shadow.z > object_nearest_light + 0.002f)
				{
					total += 0.3f;
				}
			}
		}
		total /= total_texels;
	}

	float light_factor = 1.0f - (total * dist);

	float diffuse = clamp(dot(normalize(light_position - world_pos), normal), 0, 1);
	vec4 diffuse_light = vec4(diffuse);
	diffuse_light.a = 1.0f;

	final_color = vec4(color, 1.0f) + diffuse_light * light_factor;
//	final_color *= light_factor;
}