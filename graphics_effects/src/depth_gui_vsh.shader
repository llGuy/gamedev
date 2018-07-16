#version 330 core

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 texture_coords;

out vec2 pass_texture_coords;

uniform mat4 model_matrix;
uniform mat4 projection_matrix;

void main(void)
{
	gl_Position = projection_matrix * model_matrix * vec4(vertex_position, 0.0, 1.0f);
}