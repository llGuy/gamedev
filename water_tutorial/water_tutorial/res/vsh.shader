#version 330 core

in layout(location = 0) vec3 vertex_position;
in layout(location = 1) vec3 vertex_colour;
out vec3 pass_colour;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

uniform vec4 plane;

void main(void)
{
	gl_ClipDistance[0] = dot(vec4(vertex_position, 1.0f), plane);

	vec4 view_position = view_matrix * vec4(vertex_position, 1.0f);
	vec4 projected_position = projection_matrix * view_position;
	gl_Position = projected_position;

	pass_colour = vertex_colour;
}