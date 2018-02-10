#version 430

in vec3 color;
in vec4 diffuseLight;
in vec4 specularLight;

out vec4 fragmentColor;

void main()
{
	fragmentColor = 0.8 * (clamp(diffuseLight, 0, 1) + vec4(color, 1.0) + clamp(specularLight, 0, 1));
}
