#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uvs;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec3 vertex_tangent;

out struct input_prev
{
	vec3 position;

	vec2 uvs;

	vec3 normal;

	vec4 shadow_coord;

	vec3 tangent;

	vec3 bitangent;

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
	vertex_out.position = vec3(model_matrix * vec4(vertex_position, 1.0));

	vertex_out.shadow_coord = shadow_info.shadow_bias * vec4(vertex_out.position, 1.0);

	vertex_out.uvs = vertex_uvs;

	vertex_out.tangent = vertex_tangent;

	vertex_out.normal = vertex_normal;

	vertex_out.view_position = view_matrix * vec4(vertex_out.position, 1.0);

	gl_Position = projection_matrix * vertex_out.view_position;
}