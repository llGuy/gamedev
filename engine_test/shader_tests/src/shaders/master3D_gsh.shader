layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

struct input_prev
{
	vec3 vertex_position;

	vec3 vertex_color;

	vec2 texture_coords;

	vec3 vertex_normal;
};

in input_prev vertex_out[];

out input_prev geometry_out;

void main(void)
{
	for (int i = 0; i < 3; ++i)
	{
		geometry_out.vertex_position = vertex_out[i].vertex_position;

#ifdef SHARP_NORMALS
		geometry_out.vertex_normal = vertex_out[2].vertex_normal;
#else
		geometry_out.vertex_normal = vertex_out[i].vertex_normal;
#endif

#ifdef USES_TEXTURE
		geometry_out.texture_coords = vertex_out[i].texture_coords;
#else
		geometry_out.vertex_color = vertex_out[i].vertex_color;
#endif

		gl_Position = gl_in[i].gl_Position;

		EmitVertex();
	}

	EndPrimitive();
}