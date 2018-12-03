#version 400

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

const int max_steps = 30;
const float step_value = 0.1;

vec4 binary_search(inout vec3 direction, inout vec3 result_vertex, inout float difference_depth)
{
	float sampled_depth;

	vec4 projected_coord;

	for (int i = 0; i < 5; ++i)
	{
		projected_coord	= projection_matrix	* vec4(result_vertex, 1);
		projected_coord.xy /= projected_coord.w;
		projected_coord.xy = projected_coord.xy * 0.5 + 0.5;

		sampled_depth = textureLod(view_positions, projected_coord.xy, 2).z;

		difference_depth = result_vertex.z - sampled_depth;
		direction *= 0.5;
		if (difference_depth > 0) result_vertex += direction;
		else result_vertex -= direction;
	}

	projected_coord = projection_matrix * vec4(result_vertex, 1.0f);
	projected_coord.xy /= projected_coord.w;
	projected_coord.xy = projected_coord.xy * 0.5 + 0.5;

	return vec4(projected_coord.xy, sampled_depth, 1);
}

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

		sampled_depth = textureLod(view_positions, projected_coord.xy, 2).z;

		if(sampled_depth > 1000) continue;

		/* Check the difference between the depth of the vertex the ray hit and the depth at the x-y values of the projected coord */
		float difference = result_vertex.z - sampled_depth;

		if (difference <= 0)
		{
//			return vec4(projected_coord.xy, sampled_depth, 0);
			return vec4(abs(sampled_depth));
//			return binary_search(direction, result_vertex, difference);
		}
	}

//	return vec4(projected_coord.xy, sampled_depth, 0);
	return vec4(0);
}

void main(void)
{
	vec3 view_position = (view_matrix * textureLod(view_positions, vertex_out.texture_coords, 2)).xyz;
	vec3 world_normal = texture(view_normals, vertex_out.texture_coords).xyz;
	vec3 view_normal = (view_matrix * vec4(world_normal, 0)).xyz;
	vec4 pixel_color = texture(diffuse, vertex_out.texture_coords);

	if (texture(diffuse, vertex_out.texture_coords).a > 0.5f)
	{
		/* Get direction of the ray */
		vec3 view_direction = normalize(reflect(normalize(view_position), normalize(view_normal)));
		final_color = vec4(view_direction, 1);

		vec3 result_vertex = view_position;

		vec4 color_reflected = cast_ray(view_direction, result_vertex, view_normal, world_normal);
		//final_color = color_reflected;
		//final_color = vec4(view_direction ,1);
//		final_color = mix(texture(diffuse, color_reflected.xy), pixel_color, 0.5);
		//final_color = vec4(view_position, 1);
	}
	else final_color = pixel_color;
	final_color.a = 1;
}