#version 430

in vec3 position;
in vec3 normal;
in vec3 color;

out vec4 fragmentColor;

//uniform vec3 lightPosition;
//uniform vec3 eyePosition;

void main(void)
{
	fragmentColor = vec4(color, 1.0f);
}