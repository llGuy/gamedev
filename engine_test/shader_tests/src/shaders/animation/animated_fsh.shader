#version 400

in vec2 pass_uv;
in vec3 pass_normal;

out vec4 final_color;

uniform sampler2D diffuse_map;
uniform vec3 light_dir;

void main(void)
{
	vec4 diffuse_color = texture(diffuse_map, pass_uv);

	vec3 unit_normal = normalize(pass_normal);

	float diffuse_light = max(dot(-light_dir, unit_normal), 0.0f);

	final_color = diffuse_color = diffuse_light;
}