#version 330 core

out vec4 final_color;

in vec3 final_normal;
in vec3 final_position;
in vec3 color;

const vec3 light_position = vec3(0.0f, 1000.0f, 0.0f);

vec4 get_diffuse(void)
{
	vec3 normal = vec3(0.0f, 1.0f, 0.0f);
	vec3 light_vector = normalize(light_position - final_position);
	float d = dot(light_vector, normal);
	return vec4(d, d, d, 1.0f) * 0.8f;
}

void main(void)
{
	final_color = vec4(color, 1.0f) + get_diffuse();
	
}