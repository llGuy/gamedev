#version 330 core

layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 texture_coords;
out vec2 blur_texture_coords[11];

uniform float target_width;

void main(void)
{
	gl_Position = vec4(vertex_position, 0.0f, 1.0f);
//	vec2 texture_coords = vertex_position * 0.5f + 0.5f;
//	texture_coords /= 2;
//	texture_coords.y
	
//	vec2 t = texture_coords * 2;

	float pixel_size = 1.0f / target_width;

	for (int i = -5; i < 5; ++i)
	{
		blur_texture_coords[i + 5] = texture_coords + vec2(pixel_size * i, 0.0f);
	}
}