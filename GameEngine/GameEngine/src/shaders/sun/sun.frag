#version 400 

layout(location = 0) out vec4 final_color;
layout(location = 1) out vec4 bright_color;
layout(location = 3) out vec4 view_normal;
layout(location = 4) out vec4 sun_color;

in VS_DATA {
	vec2 uvs;
} fs_in;

uniform sampler2D diffuse;

void main(void)
{
	final_color = sun_color = texture(diffuse, fs_in.uvs);

	float bright_colorness = (final_color.r * 0.2126) + (final_color.g * 0.7152) + (final_color.b * 0.722);

	view_normal = vec4(10);
	
	if (bright_colorness > 0.9)
	{
		bright_color = final_color;
	}
	else
	{
		bright_color = vec4(0);
	}
}