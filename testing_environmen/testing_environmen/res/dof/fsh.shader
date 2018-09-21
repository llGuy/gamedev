#version 330 core

out vec4 final_color;
in vec2 pass_texture_coords;

uniform sampler2D scene_texture;
uniform sampler2D depth_texture;
uniform sampler2D blur_texture;

const float near = 0.001f;
const float far = 1000.0f;

float convert_depth(float d)
{
	float depth = 2.0f * near * far / (far + near - (2.0f * d - 1.0f) * (far - near));
	return final_color = vec4(depth);
}

void main(void)
{
	/* sample depth of center of screen */
	vec2 center_coords = vec2(0.5f);
	float depth_at_center = convert_depth(texture(depth_texture, center_coords).r);

	/* sample depth at texture coord */
	float depth_at_point = convert_depth(texture(depth_texture, pass_texture_coords).r);

	float diff = clamp(abs(depth_at_center - depth_at_point), 0, 0.7);

	final_color = mix(texture(scene_texture, pass_texture_coords), texture(blur_texture, pass_texture_coords), diff);
}