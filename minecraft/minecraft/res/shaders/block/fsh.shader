#version 330 core

uniform sampler2D diffuse_texture;
uniform vec3 light_position;
uniform vec3 eye_position;
uniform vec3 sky_color;

out vec4 fragment_color;

in vec3 normal;
in float texture_data;
in float visibility;
in vec2 texture_coords;
in vec3 vertex_position;

float Diffuse(vec3 light_vector)
{
	float brightness = dot(light_vector, normal);
	return brightness * 0.3f;
}

float Specular(vec3 light_vector)
{
	vec3 reflected_light_world = reflect(-light_vector, normal);
	vec3 eye_vector = normalize(eye_position - vertex_position);
	float specularity = dot(reflected_light_world, eye_vector);
	specularity = specularity * specularity * specularity;
	return specularity;
}

vec4 Grass(void)
{
	return texture2D(diffuse_texture, texture_coords) + vec4(0.0f / 255.0f, 198.0f / 255.0f, 0.0f, 1.0f) * 0.4f;
}

vec4 Leaves(void)
{
	vec4 v = texture2D(diffuse_texture, texture_coords);
	vec4 a;
	if (v.a == 0.0f) a = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	else a = vec4(0.0f / 255.0f, 140.0f / 255.0f, 0.0f / 255.0f, 1.0f) * 0.4f;
	return v + a;
}

vec4 FragmentColor(void)
{
	if (abs(texture_data - 0.0f) < 0.0001f) return Grass();
	if (abs(texture_data - 53.0f) < 0.0001f || abs(texture_data - 52.0f) < 0.0001f) return Leaves();
	return texture2D(diffuse_texture, texture_coords);
}

void main()
{


	vec3 light_vector = normalize(light_position - vertex_position);
	float diffuse_brightness = Diffuse(light_vector);
	float specular_brightness = Specular(light_vector);

	vec4 diffuse = vec4(diffuse_brightness, diffuse_brightness, diffuse_brightness, 1.0f);
	vec4 specular = vec4(specular_brightness, specular_brightness, specular_brightness, 1.0f);

	fragment_color = clamp(diffuse, 0, 1) * 0.4f + FragmentColor() + clamp(specular, 0, 1) * 0.2f;
	//fragment_color = mix(vec4(sky_color, 1.0), fragment_color, visibility);
}