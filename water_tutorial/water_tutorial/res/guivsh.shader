#version 330 core

in layout(location = 0) vec2 vertex_position;
in layout(location = 1) vec2 texture_coordinate;
out vec2 pass_texture_coordinate;

uniform mat4 projection_matrix;

void main(void)
{
	gl_Position = /*projection_matrix* */vec4(vertex_position, 0.0f, 1.0f);
	pass_texture_coordinate = texture_coordinate;
}