layout(location = 0) out vec4 final_color;

in vec3 direction;

uniform samplerCube cube_map;

void main(void)
{
	final_color = texture(cube_map, direction);
}