uniform sampler2D diffuse;
uniform sampler2D depth;
/* 3D positions and normals in view-space */
uniform sampler2D view_positions;
uniform sampler2D view_normals;
/* Cube map to use if no reflections are available */
uniform samplerCube cube_map;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform vec3 camera_position;

noperspective in struct input_prev
{
	vec2 texture_coords;
}
vertex_out;

layout(location = 0) out vec4 final_color;

const int max_steps = 20;
const float step_value = 0.1;

vec4 cast_ray(inout vec3 direction, inout vec3 result_vertex, in vec3 normal, in vec3 world_normal)
{
	direction *= step_value;

	vec4 projected_coord;
	float sampled_depth;

	for (int i = 0; i < max_steps; i++)
	{
		result_vertex += direction;

		projected_coord = projection_matrix * vec4(result_vertex, 1);
		projected_coord.xy /= projected_coord.w;

		/* Make the projected coord x-y values between 0 and 1 */
		projected_coord.xy = projected_coord.xy * 0.5 + 0.5;

		sampled_depth = texture(view_positions, projected_coord.xy).z;

		/* Check the difference between the depth of the vertex the ray hit and the depth at the x-y values of the projected coord */
		float difference = sampled_depth - projected_coord.z;

		if (projected_coord.z > sampled_depth)
		{
			//final_color = projected_coord.rgba;
//final_color = vec4(projected_coord.xy, 0, 1);

//final_color = vec4(projected_coord.zzz, 1);
			
/* Found the pixel */
//return texture(diffuse, vec2(projected_coord.xy));
///return vec4(1 - float(i) / float(max_steps - 1));
			if (i == 0)
			{
				return vec4(1);
			}
		}
	}

	return vec4(0);
}


void main(void)
{
	vec3 view_position = texture(view_positions, vertex_out.texture_coords).xyz;
	vec3 world_normal = texture(view_normals, vertex_out.texture_coords).xyz;
	vec3 view_normal = (view_matrix * vec4(world_normal, 0)).xyz;
	vec4 pixel_color = texture(diffuse, vertex_out.texture_coords);

	if (texture(diffuse, vertex_out.texture_coords).a > 0.5f)
	{
		/* Get direction of the ray */
		vec3 view_direction = reflect(normalize(view_position), normalize(view_normal));

		vec3 result_vertex = view_position;

		vec4 color_reflected = cast_ray(view_direction, result_vertex, view_normal, world_normal);

		final_color = color_reflected;
	}
	else final_color = pixel_color;
	final_color.a = 1;
}
