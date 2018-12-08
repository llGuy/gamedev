layout(location = 0) out vec4 final_color;
layout(location = 1) out vec4 bright_color;
layout(location = 2) out vec4 view_positions;
layout(location = 3) out vec4 view_normals;

/* determine whether is linked to gsh or not */
#ifdef LINKED_TO_GSH

in struct input_prev
{
	vec3 vertex_position;

	vec3 vertex_color;

	vec2 texture_coords;

	vec3 vertex_normal;

	vec4 shadow_coord;

	vec4 view_position;
}
geometry_out;

#else

in struct input_prev
{
	vec3 vertex_position;

	vec3 vertex_color;

	vec2 texture_coords;

	vec3 vertex_normal;

	vec4 shadow_coord;




	vec4 view_position;
} 
vertex_out;

#endif

layout(std140, row_major) uniform light
{
    vec4 light_position;
	vec4 ambient_intensity;
	vec4 diffuse_intensity;
	vec4 specular_intensity;
} 
light_info;

layout(std140, row_major) uniform material
{
	vec4 ambient_reflectivity;
	vec4 diffuse_reflectivity;
	vec4 specular_reflectivity;
	float shininess_factor;
	float reflect_factor;
}
material_info;

layout(std140) uniform shadow_data
{
	mat4 shadow_bias;

	float transition_distance;
	float shadow_distance;
	float map_size;
	float pcf_count;
} 
shadow_info;

uniform sampler2D diffuse;
uniform sampler2D shadow_map;
uniform samplerCube environment;

uniform vec3 camera_position;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform int lighting;

void apply_ambient(inout vec4 color)
{
	color = light_info.ambient_intensity * material_info.ambient_reflectivity * color;
}

void apply_diffuse(float light_factor, vec3 eye_vector, vec3 light_vector, vec3 vertex_normal, inout vec4 color)
{
	float diffuse = clamp(dot(light_vector, vertex_normal), 0, 1);

	color = vec4(diffuse) * light_info.diffuse_intensity * material_info.diffuse_reflectivity * light_factor + color;
}

/*subroutine float specularity(vec3 eye_vector, vec3 light_vector, vec3 vertex_normal, inout vec4 color);
subroutine(specularity) float no_specular(vec3 eye_vector, vec3 light_vector, vec3 vertex_normal, inout vec4 color)
{
	return 0.0f;
}*/
float apply_specular(float light_factor, vec3 eye_vector, vec3 light_vector, vec3 vertex_normal, inout vec4 color)
{
	vec3 reflected_light = reflect(normalize(light_vector), vertex_normal);
	float specular = clamp(dot(reflected_light, normalize(eye_vector)), 0, 1);
	specular = pow(specular, material_info.shininess_factor);

	color = vec4(specular) * light_info.specular_intensity * material_info.specular_reflectivity * light_factor + color;

	return specular;
}

//subroutine uniform specularity apply_specularity;

void apply_reflection(float light_factor, float specular, vec3 eye_vector, vec3 light_vector, vec3 vertex_normal, inout vec4 color)
{
	vec3 result = reflect(-eye_vector, vertex_normal);

	vec4 envi_color = texture(environment, result);

	color += envi_color * specular * light_factor;

	color = mix(color, envi_color, material_info.reflect_factor);
}

float get_shadow_value(in vec3 world_pos, in vec4 shadow_coord)
{
	float total_texels = (shadow_info.pcf_count * 2.0f + 1.0f) * (shadow_info.pcf_count * 2.0f + 1.0f);

	float dist = distance(world_pos, camera_position);

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

void calculate_bright_color(void)
{
	float brightness = (final_color.r * 0.2126) + (final_color.g * 0.7152) + (final_color.b * 0.722);
	if (brightness > 0.7)
	{
		bright_color = final_color * brightness;
	}
	else
	{
		bright_color = vec4(0);
	}
}

float brightness(vec4 color)
{
	return (color.r * 0.2126) + (color.g * 0.7152) + (color.b * 0.0722);
}

void main(void)
{
#ifdef LINKED_TO_GSH
	input_prev input_data = geometry_out;
#else
	input_prev input_data = vertex_out;
#endif
#ifdef USES_TEXTURE
	final_color = texture(diffuse, input_data.texture_coords);
#endif

#ifdef USES_COLOR
	final_color = vec4(input_data.vertex_color, 1.0f);
#endif

	if (lighting == 1)
	{
		vec3 light_vector = normalize(vec3(light_info.light_position));
		vec3 eye_vector = normalize(camera_position - input_data.vertex_position);

		float light_factor = get_shadow_value(input_data.vertex_position, input_data.shadow_coord);

		apply_ambient(final_color);
		apply_diffuse(light_factor, eye_vector, light_vector, input_data.vertex_normal, final_color);
		//float specularity = apply_specular(eye_vector, light_vector, input_data.vertex_normal, final_color);
		float specularity = apply_specular(light_factor, eye_vector, light_vector, input_data.vertex_normal, final_color);

		apply_reflection(light_factor, specularity, eye_vector, light_vector, input_data.vertex_normal, final_color);

		calculate_bright_color();
	}

	view_positions = view_matrix * vec4(input_data.vertex_position, 1.0f);

	view_normals = view_matrix * vec4(input_data.vertex_normal, 0.0f);
	view_normals.a = 1;


	final_color.a = 1;
}