#version 330 core

uniform sampler2D diffuse_texture;

in vec2 tdata;

out vec4 fragment_color;

void main()
{
	fragment_color = texture2D(diffuse_texture, tdata);
	//fragment_color = vec4(1.0f, 1.0f, 1.0f, 0.0f);
}