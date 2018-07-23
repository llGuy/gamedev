#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 test;
out vec3 texture_coords;

uniform vec4 plane;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

void main(void)
{
	gl_Position = projection_matrix * view_matrix * vec4(vertex_position, 1);

	gl_ClipDistance[0] = dot(vec4(vertex_position, 1.0f), plane);

	texture_coords = vertex_position;
}