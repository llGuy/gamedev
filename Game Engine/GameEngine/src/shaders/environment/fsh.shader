layout(location = 0) out vec4 final_color;
layout(location = 1) out vec4 bright_color;

in vec3 direction;

uniform samplerCube cube_map;

void calculate_bright_color(void)
{
	float brightness = (final_color.r * 0.2126) + (final_color.g * 0.7152) + (final_color.b * 0.722);
	if (brightness > 0.7)
	{
		bright_color = final_color * brightness;
	}
	else
	{
		bright_color = vec4(0);
	}
}

void main(void)
{
	final_color = texture(cube_map, direction);

	calculate_bright_color();
}