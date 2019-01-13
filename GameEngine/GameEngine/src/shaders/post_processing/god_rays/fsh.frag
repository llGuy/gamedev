#version 400

layout(location = 0) out vec4 final_color;

in VS_DATA {

	vec2 uvs;

} fs_in;

uniform sampler2D sun_only_tex;
uniform sampler2D diffuse;
uniform vec2 light_screen_pos;

const int SAMPLES = 10;
const float DENSITY = 1.0;
const float DECAY = 0.5;
const float WEIGHT = 1.0;

void main(void)
{
	if (light_screen_pos.x <= 1.0 && light_screen_pos.y <= 1.0 && light_screen_pos.x >= -1.0 && light_screen_pos.y >= -1.0)
	{
		vec2 deltaTextCoord = vec2( fs_in.uvs - light_screen_pos.xy );

    	vec2 textCoo = fs_in.uvs;
    	deltaTextCoord *= 1.0 /  float(SAMPLES);
    	float illuminationDecay = 1.0;
	
	
    	for(int i=0; i < SAMPLES; i++)
        {
                 textCoo -= deltaTextCoord;
                 vec4 a_sample = texture2D(sun_only_tex, textCoo);
			
                 a_sample *= illuminationDecay;

                 final_color += a_sample;

                 illuminationDecay *= DECAY;
         }

		 final_color = vec4(vec2(0.1 - distance(fs_in.uvs, light_screen_pos)), texture(sun_only_tex, fs_in.uvs).x, 1.0) + texture(diffuse, fs_in.uvs);

	/*final_color = vec4(0);
	vec2 blur_vector = (light_screen_pos - fs_in.uvs) * (1.0 / float(SAMPLES));
	vec2 current_uvs = fs_in.uvs;

	float illumination_decay = 1.0;

	for (int i = 0; i < SAMPLES; ++i)
	{
		current_uvs += blur_vector;

		vec4 current_color = texture(sun_only_tex, current_uvs);

		current_color *= illumination_decay * WEIGHT;

		final_color += current_color;

		illumination_decay *= DECAY;
	}
	*/
	}
	//final_color = vec4(fs_in.uvs, 0.0, 1.0);
	//final_color = vec4(blur_vector.xy, texture(sun_only_tex, fs_in.uvs).x, 1.0);
}