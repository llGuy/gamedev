#version 330 core

in layout(location = 0) vec3 vertex_position;
in layout(location = 1) vec3 vertex_color;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform vec3 light_position;
uniform vec3 eye_position;

out vec3 color;

void main(void)
{
	//vec4 model_position = model * vec4(vertex_position, 1.0f);
	vec4 view_position = view_matrix * vec4(vertex_position, 1.0f);
	vec4 projected_position = projection_matrix * view_position;

	gl_Position = projected_position;
	color = vertex_color;
}