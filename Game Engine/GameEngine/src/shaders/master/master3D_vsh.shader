/* all possible attributes */
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_coords;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec3 vertex_color;

out struct input_prev
{
	vec3 vertex_position;

	vec3 vertex_color;

	vec2 texture_coords;

	vec3 vertex_normal;

	vec4 shadow_coord;
}
vertex_out;

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 shadow_bias;

/*layout(std140, row_major) uniform shadow_data 
{
	float transition_distance;
	float shadow_distance;
	float map_size;

	mat4 shadow_bias;
} 
shadow_info;*/

void main(void)
{
	vertex_out.vertex_position = vec3(model_matrix * vec4(vertex_position, 1.0f));

#ifdef USES_COLOR
	vertex_out.vertex_color = vec3(1.0f, 0.0f, 1.0f);
#endif

#ifdef USES_TEXTURE
	vertex_out.texture_coords = texture_coords;
#endif

#ifdef USES_STATIC_NORMALS
	vertex_out.vertex_normal = vec3(model_matrix* vec4(vertex_normal, 0.0f));
#endif

	vertex_out.shadow_coord = shadow_bias * vec4(vertex_out.vertex_position, 1.0f);

	gl_Position = projection_matrix * view_matrix * vec4(vertex_out.vertex_position, 1.0f);
}