#version 330 core

layout(location = 0) in vec2 vertex_position;

uniform vec3 model_color;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main(void)
{
	gl_Position = projection_matrix * view_matrix * vec4(vertex_position, 0.0f, 1.0f);
}