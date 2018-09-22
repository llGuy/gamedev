#version 330 core

layout(location = 0) in vec3 vertex_position;
out vec3 pass_color;

uniform mat4 projection_matrix;
uniform vec3 camera_direction;
uniform vec4 top;
uniform vec4 bottom;
uniform mat4 model_matrix;

float calculate_factor(float sign_of_y)
{
	return 0.5f * (1.0f - sign_of_y);
}

void main(void)
{
	float pi = 3.14159265359;

	vec4 world = model_matrix * vec4(vertex_position, 1.0f);
	gl_Position = projection_matrix * world;

	float mix_factor = sin(normalize(camera_direction).y * (pi / 2.0f));

	if (gl_Position.y < 0.0f)
	{
		pass_color = vec3(bottom);
		if (mix_factor > 0.0f) pass_color = vec3(mix(vec4(pass_color, 1.0f), top, mix_factor));
	}
	else
	{
		pass_color = vec3(top);
		if (mix_factor < 0.0f) pass_color = vec3(mix(vec4(pass_color, 1.0f), bottom, -mix_factor));
	}
}