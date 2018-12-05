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
uniform int num_marches;

noperspective in struct input_prev
{
	vec2 texture_coords;
}
vertex_out;

layout(location = 0) out vec4 final_color;

const int max_steps = 10;
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

vec4 cast_ray(inout vec3 direction, inout vec3 hit_coord, in vec3 view_normal)
{
	vec3 current_position = hit_coord;

	vec3 ray_dir = direction * 0.1;

	vec4 projected_coord;
	float sampled_depth;

	for (int i = 0; i < num_marches; i++)
	{
		current_position += ray_dir;

		projected_coord = projection_matrix * vec4(current_position, 1);
		projected_coord.xy /= projected_coord.w;

		/* Make the projected coord x-y values between 0 and 1 */
		projected_coord.xy = projected_coord.xy * 0.5 + 0.5;

		sampled_depth = textureLod(view_positions, projected_coord.xy, 2).z;

		if(sampled_depth > 1000) return vec4(1, 0, 0, 1);

		/* Check the difference between the depth of the vertex the ray hit and the depth at the x-y values of the projected coord */
		float difference = current_position.z - sampled_depth;

		//if ((ray_dir.z - difference) < 1.2)
		//{
		if (current_position.z <= sampled_depth)
		{
			//return vec4(projected_coord.xy, sampled_depth, 0);
			//return texture(diffuse, projected_coord.xy);
			vec4 something = (projection_matrix * vec4(hit_coord, 1.0));
			something.xy /= something.w;
			return vec4(projected_coord.xy - something.xy, 0, 1);
//			return vec4(abs(difference));

			//return vec4(abs(sampled_depth));
//			return binary_search(direction, result_vertex, difference);
		}
		//}
	}

	//return texture(diffuse, projected_coord.xy);
				vec4 something = (projection_matrix * vec4(hit_coord, 1.0));
			something.xy /= something.w;
			return vec4(projected_coord.xy - something.xy, 0, 1);
	return vec4(0);
}

void main(void)
{
	vec3 view_position = (textureLod(view_positions, vertex_out.texture_coords, 2)).xyz;
	vec3 view_normal = texture(view_normals, vertex_out.texture_coords).xyz;
//	vec3 view_normal = (textureLod(view_normals, vertex_out.texture_coords, 2)).xyz;
	vec4 pixel_color = texture(diffuse, vertex_out.texture_coords);

	if (texture(diffuse, vertex_out.texture_coords).a > 0.5f)
	{
		/* Get direction of the ray */
		vec3 view_direction = normalize(reflect(normalize(view_position), normalize(view_normal)));
		//view_direction.z *= -1;
		//final_color = vec4(view_direction, 1);
		 
		vec3 hit_coord = view_position;

		vec3 ray_dir = view_direction * 0.1;
		//vec3 ray_dir = view_direction * max(0.1, -view_position.z);
		vec4 projected_coord;
		float sampled_depth;

		bool success = false;

		for (int i = 0; i < 30; i++)
		{
			hit_coord += ray_dir;

			projected_coord = projection_matrix * vec4(hit_coord, 1);
			projected_coord.xy /= projected_coord.w;

			/* Make the projected coord x-y values between 0 and 1 */
			projected_coord.xy = projected_coord.xy * 0.5 + 0.5;

			sampled_depth = textureLod(view_positions, projected_coord.xy, 2).z;

			/* Check the difference between the depth of the vertex the ray hit and the depth at the x-y values of the projected coord */
			float difference = hit_coord.z - sampled_depth;

			if (sampled_depth <= view_position.z)
			{
			if (hit_coord.z <= sampled_depth)
			{
				projected_coord = binary_search(ray_dir, hit_coord, difference);
				//projected_coord = vec4(projected_coord.xy, sampled_depth, 0);
//				projected_coord = vec4(0);
				success = true;
				break;
			}
			}
		}
		//final_color = vec4(projected_coord.xy, 0, 1);
		//final_color = vec4(view_position, 1);
//		if (!success) final_color = vec4(0);

//		final_color = texture(diffuse, projected_coord.xy);
		if (success) final_color = texture(diffuse, projected_coord.xy);
		else final_color = pixel_color;
		//if (!success) final_color = vec4(0);
	}
	else final_color = pixel_color;
	final_color.a = 1;

//	final_color.xyz = view_position.zzz;
//final_color = vec4(view_position.y);
}