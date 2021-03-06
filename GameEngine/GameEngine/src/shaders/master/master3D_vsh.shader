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

	vec4 view_position;
}
vertex_out;

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

layout(std140) uniform shadow_data 
{
	mat4 shadow_bias;

	float transition_distance;
	float shadow_distance;
	float map_size;
	float pcf_count;
} 
shadow_info;

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

	vertex_out.shadow_coord = shadow_info.shadow_bias * vec4(vertex_out.vertex_position, 1.0f);

	gl_Position = projection_matrix * view_matrix * vec4(vertex_out.vertex_position, 1.0f);

	vertex_out.view_position = view_matrix * vec4(vertex_out.vertex_position, 1.0f);
}