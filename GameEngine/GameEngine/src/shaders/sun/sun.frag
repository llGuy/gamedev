#version 400 

in VS_DATA {
	vec2 uvs;
} fs_in;

uniform sampler2D diffuse;

void main(void)
{
	final_color = texture(diffuse, fs_in.uvs);
}