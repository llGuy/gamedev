layout(location = 0) out vec4 final_color;

/* determine whether is linked to gsh or not */
#ifdef LINKED_TO_GSH

in struct input_prev
{
	vec3 vertex_position;

	vec3 vertex_color;

	vec2 texture_coords;

	vec3 vertex_normal;
}
geometry_out;

#else

in struct input_prev
{
	vec3 vertex_position;

	vec3 vertex_color;

	vec2 texture_coords;

	vec3 vertex_normal;
} 
vertex_out;

#endif

uniform sampler2D diffuse;

void main(void)
{
#ifdef LINKED_TO_GSH
	input_prev input_data = geometry_out;
#else
	input_prev input_data = vertex_out;
#endif

#ifdef USES_TEXTURE
	final_color = texture(diffuse, input_data.texture_coords);

	final_color = vec4(input_data.vertex_normal, 1.0f);
#endif

#ifdef USES_COLOR
	final_color = vec4(input_data.vertex_color, 1.0f);
#endif
}