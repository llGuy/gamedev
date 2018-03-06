#version 330 core

in layout(location = 0) vec2 vertex_position;
in layout(location = 1) vec2 texture_data;

out vec2 tdata;

uniform mat4 model_matrix;

void main()
{
	tdata = texture_data;
	gl_Position = vec4(vertex_position, 0.0f, 1.0f);
}