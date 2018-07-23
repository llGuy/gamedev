#version 330 core

in  vec2 pass_texture_coords;
out vec4 final_color;

uniform sampler2D diffuse;

const float near = 0.001f;
const float far = 1000.0f;

float convert_depth(float raw)
{
	return 2.0f * near * far / (far + near - (2.0f * raw - 1.0f) * (far - near));
}


vec4 get_depth_color(void)
{
	float d = convert_depth(texture(diffuse, pass_texture_coords).x) / 100.0f;

	return vec4(d, d, d, 1.0f);
}

void main(void)
{
	//final_color = get_depth_color();
	final_color = texture(diffuse, pass_texture_coords);
}