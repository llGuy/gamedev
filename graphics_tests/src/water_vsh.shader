#version 330 core

layout(location = 0) in vec3 vertex_position;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

void main(void)
{
	gl_ClipDistance[0] = -1;
	gl_Position = projection_matrix * view_matrix * vec4(vertex_position, 1.0f);
}