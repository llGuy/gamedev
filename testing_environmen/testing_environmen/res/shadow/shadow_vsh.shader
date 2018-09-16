#version 330 core

layout(location = 0) in vec3 vertex_position;

uniform mat4 vp;
uniform mat4 model;

void main(void)
{
	gl_Position = vp * model * vec4(vertex_position, 1.0f);
}