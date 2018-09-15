#version 330 core

layout(location = 0) in vec2 vertex_position;


uniform mat4 model_matrix;

void main(void)
{
	gl_Position = model_matrix * vec4(vertex_position, 0, 1);
	out_texture_coords = texture_coords;
}