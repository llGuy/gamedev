#version 330 core

in layout(location = 0) vec3 vertex_position;
out vec3 pass_vertex_color;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

void main()
{
	vec4 view_position = view_matrix * vec4(vertex_position, 1.0f);
	gl_Position = projection_matrix * view_position;
	pass_vertex_color = vec3(0.0f, 1.0f, 0.0f);
}