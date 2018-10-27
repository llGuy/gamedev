layout(location = 0) out vec4 final_color;

in struct 
{
	vec2 texture_coords;

	float texture_slot;
}
vertex_out;

uniform sampler2D diffuse[32];

vec4 calculate_color(void)
{
	return texture(diffuse[int(vertex_out.texture_slot)], vertex_out.texture_coords);
}

void main(void)
{
	final_color = calculate_color();
}