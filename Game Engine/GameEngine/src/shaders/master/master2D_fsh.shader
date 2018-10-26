layout(location = 0) out vec4 final_color;

in struct 
{
	vec3 vertex_color;

	vec2 texture_coords;
}
vertex_out;

uniform sampler2D diffuse;

uniform vec3 color;

vec4 calculate_color(void)
{
#ifdef USES_COLOR
	return vec4(vertex_out.vertex_color, 1.0f);
#endif

#ifdef USES_TEXTURE

#ifdef FONT
	return vec4(color, texture(diffuse, vertex_out.texture_coords).a);
#endif

	return texture(diffuse, vertex_out.texture_coords);
#endif

}

void main(void)
{
	final_color = calculate_color();
}