#version 330 core

in layout(location = 0) vec3 vertex_position;
in layout(location = 1) vec3 vertex_color;
out vec3 pass_vertex_color;
out vec4 pass_projected_position;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

void main(void)
{
	vec4 world_position = vec4(vertex_position, 1.0f);
	vec4 view_position = view_matrix * world_position;
	vec4 projected_position = projection_matrix * view_position;

	gl_Position = projected_position;
	pass_projected_position = projected_position;
	pass_vertex_color = vertex_color;
}