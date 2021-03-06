#version 330 core

layout(location = 0) in vec3 vertex_position;

uniform mat4 model_matrix;
uniform mat4 depth_mvp;

void main(void)
{
	gl_Position = depth_mvp * model_matrix * vec4(vertex_position, 1.0f);
}