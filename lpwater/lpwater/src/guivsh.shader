#version 330 core

in layout(location = 0) vec2 vertex_position;
in layout(location = 1) vec2 texture_coordinate;
out vec2 pass_texture_coordinate;

void main(void)
{
	gl_Position = vec4(vertex_position, 0.0f, 1.0f);
	pass_texture_coordinate = texture_coordinate;
}