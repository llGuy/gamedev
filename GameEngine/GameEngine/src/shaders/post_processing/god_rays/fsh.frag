#version 400

layout(location = 0) out vec4 final_color;

in VS_DATA {

	vec2 uvs;

} fs_in;

uniform sampler2D sun_only_tex;
uniform sampler2D diffuse;
uniform vec2 light_screen_pos;

const int SAMPLES = 50;
const float DECAY = 0.9;

void main(void)
{
	final_color = vec4(0);
	vec2 blur_vector = (light_screen_pos - fs_in.uvs) * (1.0 / float(SAMPLES));
	vec2 current_uvs = fs_in.uvs;

	float illumination_decay = 1.5;

	for (int i = 0; i < SAMPLES; ++i)
	{
		illumination_decay *= DECAY;

		current_uvs += blur_vector;
		vec4 current_color = texture(sun_only_tex, current_uvs);
		current_color *= illumination_decay;
		final_color += current_color;
	}
	final_color += texture(diffuse, fs_in.uvs);
}