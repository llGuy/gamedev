layout(location = 0) out vec4 final_color;

in struct input_prev
{
	vec2 texture_coords;
}
vertex_out;

uniform sampler2D diffuse;
uniform sampler2D brightness;

void main(void)
{
	final_color = texture(diffuse, vertex_out.texture_coords) + 0.15 * texture(brightness, vertex_out.texture_coords);
}