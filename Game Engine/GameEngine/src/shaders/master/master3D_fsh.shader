layout(location = 0) out vec4 final_color;
/* for bloom effect */
//layout(location = 1) out vec4 final_bright_colors;

/* determine whether is linked to gsh or not */
#ifdef LINKED_TO_GSH

in struct input_prev
{
	vec3 vertex_position;

	vec3 vertex_color;

	vec2 texture_coords;

	vec3 vertex_normal;
}
geometry_out;

#else

in struct input_prev
{
	vec3 vertex_position;

	vec3 vertex_color;

	vec2 texture_coords;

	vec3 vertex_normal;
} 
vertex_out;

#endif

uniform struct material
{
	vec3 ambient_reflectivity;
	vec3 diffuse_reflectivity;
	vec3 specular_reflectivity;
	float shininess_factor;
	float reflect_factor;
} 
material_info;

uniform struct light
{
	vec3 light_position;
	vec3 ambient_intensity;
	vec3 diffuse_intensity;
	vec3 specular_intensity;
}
light_info;

uniform sampler2D diffuse;
uniform samplerCube environment;

uniform vec3 camera_position;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

void apply_ambient(inout vec4 color)
{
	color = vec4(light_info.ambient_intensity, 1.0f) * vec4(material_info.ambient_reflectivity, 1.0f) * color;
}

void apply_diffuse(vec3 light_vector, vec3 vertex_normal, inout vec4 color)
{
	float diffuse = clamp(dot(light_vector, vertex_normal), 0, 1);

	color = vec4(diffuse) * vec4(light_info.diffuse_intensity, 1.0f) * vec4(material_info.diffuse_reflectivity, 1.0f) + color;
}

float apply_specular(vec3 eye_vector, vec3 light_vector, vec3 vertex_normal, inout vec4 color)
{
	vec3 reflected_light = reflect(normalize(light_vector), vertex_normal);
	float specular = clamp(dot(reflected_light, normalize(eye_vector)), 0, 1);
	specular = pow(specular, material_info.shininess_factor);

	color = vec4(specular) * vec4(light_info.specular_intensity, 1.0f) * vec4(material_info.specular_reflectivity, 1.0f) + color;

	return specular;
}

void apply_reflection(float specular, vec3 eye_vector, vec3 light_vector, vec3 vertex_normal, inout vec4 color)
{
	vec3 result = reflect(-eye_vector, vertex_normal);

	vec4 envi_color = texture(environment, result);

	color += envi_color * specular;

	color = mix(color, envi_color, material_info.reflect_factor);
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

	//vec3 light_vector = normalize(light_info.light_position - input_data.vertex_position);
	vec3 light_vector = normalize(-light_info.light_position);
	vec3 eye_vector = normalize(camera_position - input_data.vertex_position);

	
	apply_ambient(final_color);
	apply_diffuse(light_vector, input_data.vertex_normal, final_color);
	float specularity = apply_specular(eye_vector, light_vector, input_data.vertex_normal, final_color);

//	apply_reflection(specularity, eye_vector, light_vector, input_data.vertex_normal, final_color);

//	final_bright_colors = final_color * brightness(final_color);
}