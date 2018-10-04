#version 330 core

in vec2 out_texture_coords;
out vec4 final_color;

uniform sampler2D diffuse;

const float near = 0.1f;
const float far = 1000.0f;

void main(void)
{
	vec4 color = texture(diffuse, out_texture_coords);
	float raw = color.r;
	float depth = 2.0f * near * far / (far + near - (2.0f * raw - 1.0f) * (far - near));
	final_color = vec4(depth);
}