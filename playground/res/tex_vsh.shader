#version 330 core

layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 texture_coords;
out vec2 pass_texture_coords;

uniform mat4 projection_matrix;
uniform vec2 translation;

void main(void)
{
	vec4 position = vec4(translation + vertex_position, 0.0f, 1.0f);
	pass_texture_coords = texture_coords;
	gl_Position = position;
}