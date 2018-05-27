#version 330 core

in layout(location = 0) vec3 vertex_position;

out vec3 pass_vertex_color;
out vec3 pass_vertex_world_position;

uniform vec3 color;
uniform mat4 matrix_view;
uniform mat4 matrix_projection;

void main(void)
{
	vec4 view_position = matrix_view * vec4(vertex_position, 1.0f);
	gl_Position = matrix_projection * view_position;

	pass_vertex_color = color;
	pass_vertex_world_position = vertex_position;
}