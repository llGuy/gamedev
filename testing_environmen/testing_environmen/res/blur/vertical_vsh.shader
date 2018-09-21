#version 330 core

layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 texture_coords;
out vec2 blur_texture_coords[11];

uniform float target_height;

void main(void)
{
	gl_Position = vec4(vertex_position, 0, 1);

	float pixel_size = 1.0f / target_height;

	for (int i = -5; i < 5; ++i)
	{
		blur_texture_coords[i + 5] = texture_coords + vec2(0, pixel_size * i);
	}
}