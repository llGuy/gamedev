#version 330 core

in layout(location = 0) vec3 vertex_position;
in layout(location = 1) vec2 texture_coordinate;
out vec2 pass_texture_coordinate;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main(void)
{
	vec4 view_position = view_matrix * vec4(vertex_position, 1.0f);
	vec4 projected_position = projection_matrix * view_position;
	gl_Position = projected_position;
}