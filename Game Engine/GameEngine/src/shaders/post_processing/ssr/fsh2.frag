#version 400 

layout(location = 0) out vec4 final_color;

uniform sampler2D diffuse;
uniform sampler2D depth;
uniform sampler2D view_positions;
uniform sampler2D view_normals;
uniform samplerCube cube_map;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 inverse_view_matrix;

uniform vec3 camera_position;
uniform int num_marches;

noperspective in struct input_prev
{
	vec2 texture_coords;
}
vertex_out;



vec3 hash33(vec3 p3)
{
	p3 = fract(p3 * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yxz + 19.19);
    return fract((p3.xxy + p3.yxx) * p3.zyx);
}

vec3 binary_search(inout vec3 dir, inout vec3 hit_coord, inout float depth_difference)
{
	float depth;

	vec4 projected_coord;

	for (int i = 0; i < 8; ++i)
	{
		projected_coord = projection_matrix * vec4(hit_coord, 1.0);
		projected_coord.xy /= projected_coord.w;
		projected_coord.xy = projected_coord.xy * 0.5 + 0.5;

		depth = textureLod(view_positions, projected_coord.xy, 2).z;

		depth_difference = hit_coord.z - depth_difference;

		dir *= 0.5;
		if (depth_difference > 0.0) hit_coord += dir;
		else hit_coord -= dir;
	}

	projected_coord = projection_matrix * vec4(hit_coord, 1.0);
	projected_coord.xy /= projected_coord.w;
	projected_coord.xy = projected_coord.xy * 0.5 + 0.5;

	return vec3(projected_coord.xy, depth);
}

vec4 ray_cast(inout vec3 direction, inout vec3 hit_coord, out float depth_difference, out bool success)
{
	vec3 original_coord = hit_coord;

	direction *= 0.1;

	vec4 projected_coord;
	float sampled_depth;

	for (int i = 0; i < 20; ++i)
	{
		hit_coord += direction;

		projected_coord = projection_matrix * vec4(hit_coord, 1.0);

		projected_coord.xy /= projected_coord.w;
		projected_coord.xy = projected_coord.xy * 0.5 + 0.5;

		sampled_depth = textureLod(view_positions, projected_coord.xy, 2).z;

		if (sampled_depth > 1000.0) continue;

		depth_difference = hit_coord.z - sampled_depth;

		if (sampled_depth <= original_coord.z)
		{
			if ((projected_coord.x > 0) && (projected_coord.y > 0) && (projected_coord.x < 1) && (projected_coord.y < 1))
			{
			if ((direction.z - depth_difference) < 1.2)
			{
				if (depth_difference <= 0.0)
				{
					vec4 result;
					result = vec4(binary_search(direction, hit_coord, depth_difference), 1.0);

					success = true;

					return result;
				}
			}
			}
		}
	}

	return vec4(projected_coord.xy, sampled_depth, 0.0);
}

vec3 fresnel_schlick(float cos_theta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cos_theta, 5.0);
}

void apply_cube_map_reflection(in vec3 vs_eye_vector, in vec3 vs_normal)
{
	vec3 result = reflect(-normalize(vs_eye_vector), vs_normal);
	vec3 ws_reflect = vec3(inverse_view_matrix * vec4(result, 1.0));

	vec3 ws_eye_vector = vec3(inverse_view_matrix * vec4(vs_eye_vector, 1.0));
	vec3 ws_normal     = vec3(inverse_view_matrix * vec4(vs_normal, 1.0));
	
	vec3 reflect_dir = normalize(reflect(-normalize(ws_eye_vector), ws_normal));

	vec4 envi_color = texture(cube_map, reflect_dir);

	//	final_color += envi_color;
	//	final_color = mix(final_color, envi_color, 0.5);
	//	final_color = envi_color;
//	final_color = vec4(, 1.0);
}

void main(void)
{
	vec3 view_position = (textureLod(view_positions, vertex_out.texture_coords, 2)).xyz;
	vec3 view_normal = (textureLod(view_normals, vertex_out.texture_coords, 2)).xyz;
	vec4 pixel_color = texture(diffuse, vertex_out.texture_coords);

	vec3 original_position = view_position;

	if (pixel_color.a > 0.5)
	{
		bool hit = false;

		vec3 F0 = vec3(0.04);
		F0 = mix(F0, pixel_color.rgb, 0.4);
		vec3 fresnel = fresnel_schlick(max(dot(view_normal, normalize(view_position)), 0.0), F0);

		float ddepth;
		vec3 world_position = vec3(inverse_view_matrix * vec4(view_position, 1.0));
		vec3 jitt = mix(vec3(0.0), vec3(hash33(view_position)), 0.5);
		vec3 ray_dir = normalize(reflect(normalize(view_position), normalize(view_normal)));
		ray_dir = jitt + ray_dir * max(0.1, -view_position.z);

		/* ray cast */
		vec4 coords = ray_cast(ray_dir, view_position, ddepth, hit);

		vec2 d_coords = smoothstep(0.2, 0.6, abs(vec2(0.5) - coords.xy));

		float screen_edge_factor = clamp(1.0 - (d_coords.x + d_coords.y), 0.0, 1.0);
		
		float reflection_multiplier = pow(0.4, 3.0) *
			screen_edge_factor *
			-ray_dir.z;

		vec3 ssr = textureLod(diffuse, coords.xy, 0).rgb * clamp(reflection_multiplier, 0.0, 0.9) * fresnel;

		final_color = vec4(ssr, 0.4);

		if(hit) final_color = mix(texture(diffuse, coords.xy), pixel_color, 0.5);
		else final_color = pixel_color;

		//apply_cube_map_reflection(-original_position, view_normal);
	}
	else final_color = pixel_color;
}