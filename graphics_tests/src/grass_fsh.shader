#version 330 core

in vec3 world_pos;
in vec3 to_camera;
in vec3 light_vector;
in vec2 pass_texture_coords;
out vec4 final_color;

uniform vec3 light_position;
uniform sampler2D normal_texture;
uniform sampler2D grass_texture;

void main(void)
{
	vec3 normal_sample = texture(normal_texture, pass_texture_coords).rgb;
	vec3 normal = vec3(normal_sample.r * 2.0f - 1.0f, normal_sample.b, normal_sample.g * 2.0f - 1.0f);

	vec3 reflected_light = reflect(normalize(light_vector), normal);
	float specular = dot(reflected_light, normalize(to_camera));
	specular = pow(specular, 50.0f);
	vec3 specular_highlights = vec3(0.5f) * specular * 0.2f;

	float diffuse = dot(normalize(light_position - world_pos), normal) * 0.5f;
	vec4 brightness = vec4(diffuse, diffuse, diffuse, 1.0f);

	final_color = texture(grass_texture, pass_texture_coords) + brightness * 0.8f + vec4(specular_highlights, 1.0f) * 0.000004f;
}