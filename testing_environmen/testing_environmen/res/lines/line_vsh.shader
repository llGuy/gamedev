#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
out vec4 pass_vertex_color;

uniform mat4 projection;
uniform mat4 view;

void main(void)
{
	gl_Position = projection * view * vec4(vertex_position, 1.0f);

	pass_vertex_color = vec4(vertex_color, 1.0f);
}