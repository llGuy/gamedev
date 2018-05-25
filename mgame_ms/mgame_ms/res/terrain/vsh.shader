#version 430

in layout(location = 0) vec3 vertex_position;
in layout(location = 1) vec3 vertex_color;
out vec3 pass_vertex_color;
out vec3 pass_vertex_world_position;

uniform vec3 light_position;
uniform mat4 matrix_view;
uniform mat4 matrix_projection;

void main(void)
{
	vec4 view_position = matrix_view * vec4(vertex_position, 1.0f);
	gl_Position = matrix_projection * view_position;

	pass_vertex_color = vertex_color;
}