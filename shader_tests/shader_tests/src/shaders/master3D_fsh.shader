#version 400

layout(location = 0) vec4 final_color;

struct input_prev
{
	vec3 vertex_position;

	vec3 vertex_color;

	vec3 texture_coords;

	vec3 vertex_normal;
};

/* determine whether is linked to gsh or not */
#ifdef LINKED_TO_GSH
in struct input_prev geometry_out;
#else
in struct input_prev vertex_out;
#endif

uniform sampler2D diffuse;

void main(void)
{
#ifdef LINKED_TO_GSH
	input_prev input = geometry_out;
#else
	input_prev input = vertex_out;
#endif

	final_color = texture(diffuse, input.texture_coords);
}