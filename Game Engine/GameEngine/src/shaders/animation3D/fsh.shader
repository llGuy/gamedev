layout(location = 0) out vec4 final_color;
layout(location = 1) out vec4 bright_color;

/* determine whether is linked to gsh or not */
#ifdef LINKED_TO_GSH

in struct input_prev
{
	vec3 vertex_position;

	vec2 texture_coords;

	vec3 vertex_normal;

	vec3 weights;
}
geometry_out;

#else

in struct input_prev
{
	vec3 vertex_position;

	vec2 texture_coords;

	vec3 vertex_normal;

	vec3 weights;
} 
vertex_out;

#endif

layout(std140, row_major) uniform material
{
	vec4 ambient_reflectivity;
	vec4 diffuse_reflectivity;
	vec4 specular_reflectivity;
	float shininess_factor;
	float reflect_factor;
}
material_info;

layout(std140, row_major) uniform light
{
	vec4 light_position;
	vec4 ambient_intensity;
	vec4 diffuse_intensity;
	vec4 specular_intensity;
}
light_info;

uniform sampler2D diffuse;
uniform samplerCube environment;

uniform vec3 camera_position;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform int lighting;

void apply_ambient(inout vec4 color)
{
	color = light_info.ambient_intensity * material_info.ambient_reflectivity * color;
}

void apply_diffuse(vec3 light_vector, vec3 vertex_normal, inout vec4 color)
{
	float diffuse = clamp(dot(light_vector, vertex_normal), 0, 1);

	color = vec4(diffuse) * light_info.diffuse_intensity * material_info.diffuse_reflectivity + color;
}

float apply_specular(vec3 eye_vector, vec3 light_vector, vec3 vertex_normal, inout vec4 color)
{
	vec3 reflected_light = reflect(normalize(light_vector), vertex_normal);
	float specular = clamp(dot(reflected_light, normalize(eye_vector)), 0, 1);
	specular = pow(specular, material_info.shininess_factor);

	color = vec4(specular) * light_info.specular_intensity * material_info.specular_reflectivity + color;

	return specular;
}

void apply_reflection(float specular, vec3 eye_vector, vec3 light_vector, vec3 vertex_normal, inout vec4 color)
{
	vec3 result = reflect(-eye_vector, vertex_normal);

	vec4 envi_color = texture(environment, result);

	color += envi_color * specular;

	color = mix(color, envi_color, material_info.reflect_factor);
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

void main(void)
{
	if (lighting == 1)
	{
#ifdef LINKED_TO_GSH
		input_prev input_data = geometry_out;
#else
		input_prev input_data = vertex_out;
#endif

#ifdef USES_TEXTURE
		final_color = texture(diffuse, input_data.texture_coords);
#endif

		vec3 light_vector = normalize(vec3(-light_info.light_position));
		vec3 eye_vector = normalize(camera_position - input_data.vertex_position);

		apply_ambient(final_color);
		apply_diffuse(light_vector, -input_data.vertex_normal, final_color);
		float specularity = apply_specular(eye_vector, light_vector, -input_data.vertex_normal, final_color);

		apply_reflection(specularity, eye_vector, light_vector, -input_data.vertex_normal, final_color);
	}
	else
	{
		final_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	calculate_bright_color();

	final_color.a = 0;
}