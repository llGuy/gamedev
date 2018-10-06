#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 3) in mat4 transform_matrix;

uniform mat4 vp;
uniform mat4 model_matrix;

void main(void)
{
	gl_Position = vp * transform_matrix * vec4(vertex_position, 1.0f);
}