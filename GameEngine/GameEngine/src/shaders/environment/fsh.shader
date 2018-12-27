layout(location = 0) out vec4 final_color;
layout(location = 1) out vec4 bright_color;
layout(location = 2) out vec4 view_positions;
layout(location = 3) out vec4 normal;

in vec3 direction;
in vec4 view_position;

uniform samplerCube cube_map;

void calculate_bright_color(void)
{
	float brightness = (final_color.r * 0.2126) + (final_color.g * 0.7152) + (final_color.b * 0.722);
	
	if (brightness > 0.9)
	{
		bright_color = final_color;
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

	view_positions = view_position;
	normal = vec4(0);

	final_color.a = 0;
}