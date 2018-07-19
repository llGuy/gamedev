#version 330 core

out vec4 final_color;
in vec4 clip_space;
in vec2 texture_coords;
in vec3 to_camera;

uniform sampler2D reflection_texture;
uniform sampler2D refraction_texture;
uniform sampler2D dudv_texture;
uniform vec3 camera_position;
uniform float move_factor;

const float wave_strength = 0.02f;

void main(void)
{
	vec2 ndc = clip_space.xy / clip_space.w / 2.0f + 0.5f;
	vec2 refraction_tex_coords = vec2(ndc.x, ndc.y);
	vec2 reflection_tex_coords = vec2(ndc.x, -ndc.y);

	vec2 distortion1 = (texture(dudv_texture, vec2(texture_coords.x, texture_coords.y)).rg * 2.0f - 1.0f) * wave_strength;
	vec2 distortion2 = (texture(dudv_texture, vec2(-texture_coords.x, texture_coords.y + move_factor)).rg * 2.0f - 1.0f) * wave_strength;
	vec2 total = distortion1 + distortion2;

	refraction_tex_coords += clamp(vec2(total.x, total.y), 0.001f, 0.999f);
	reflection_tex_coords += total;
	reflection_tex_coords.x = clamp(reflection_tex_coords.x, 0.001f, 0.999);
	reflection_tex_coords.y = clamp(reflection_tex_coords.y, -0.999, -0.001f);

	vec4 reflection_color = texture(reflection_texture, reflection_tex_coords);
	vec4 refraction_color = texture(refraction_texture, refraction_tex_coords);

	vec3 view_vector = normalize(to_camera);
	float refractive_factor = dot(view_vector, vec3(0.0f, 1.0f, 0.0f));
	refractive_factor = pow(refractive_factor, 1.0f);

	final_color = mix(reflection_color, refraction_color, refractive_factor);
	final_color = mix(final_color, vec4(0, 0.3, 0.5, 1.0), 0.2);
	//final_color = vec4(texture_coords.xy, 0.0f, 1.0f);
}