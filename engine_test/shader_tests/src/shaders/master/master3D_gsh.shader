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

uniform vec3 light_position;
uniform vec3 camera_position;

vec3 calculate_normal(int i1, int i2, int i3)
{
	vec3 diff_world_pos1 = normalize(vertex_out[i2].vertex_position - vertex_out[i1].vertex_position);
	vec3 diff_world_pos2 = normalize(vertex_out[i3].vertex_position - vertex_out[i2].vertex_position);
	return normalize(cross(diff_world_pos2, diff_world_pos1));
}

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

#ifndef DYNAMIC_NORMALS
		geometry_out.vertex_normal = calculate_normal(i, (i + 1) % 3, (i + 2) % 3); 
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