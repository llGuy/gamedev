#version 320 es

in layout(location = 0) vec3 vertex_position;

uniform vec3 model_color;
uniform mat4 matrix_model;
uniform mat4 matrix_view;
uniform mat4 matrix_projection;

void main(void)
{
	vec4 world_position = matrix_model * vec4(vertex_position, 1.0f);
	vec4 view_position = matrix_view * world_position;
	gl_Position = matrix_projection * view_position;
}
