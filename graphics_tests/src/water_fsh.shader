#version 330 core

out vec4 final_color;
in vec4 clip_space;

uniform sampler2D reflection_texture;
uniform sampler2D refraction_texture;

void main(void)
{
	vec2 ndc = clip_space.xy / clip_space.w / 2.0f + 0.5f;
	vec2 refraction_tex_coords = vec2(ndc.x, ndc.y);
	vec2 reflection_tex_coords = vec2(ndc.x, -ndc.y);

	vec4 reflection_color = texture(reflection_texture, reflection_tex_coords);
	vec4 refraction_color = texture(refraction_texture, refraction_tex_coords);

	final_color = mix(reflection_color, refraction_color, 0.5f);
}