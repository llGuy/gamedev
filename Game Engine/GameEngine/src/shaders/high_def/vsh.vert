#version 400

layout(location = 0) in vec3 vtx_pos;
layout(location = 1) in vec2 uvs;
layout(location = 2) in vec3 normal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

/* Uniform buffer for all Shadow rendering data */
layout(std140) uniform shadow_data 
{
	mat4 shadow_bias;

	float transition_distance;
	float shadow_distance;
	float map_size;
	float pcf_count;
} 
shadow_info;

out VS_DATA
{
	vec4 shadow_coord;
	vec3 ws3D_position;
	vec2 uvs;
	vec3 normal;
}
data;

void main(void)
{
	vec4 ws3D_position = model_matrix * vec4(vtx_pos, 1);
	gl_Position = projection_matrix * view_matrix * ws3D_position;

	data.shadow_coord = shadow_info.shadow_bias * ws3D_position;
	data.ws3D_position = ws3D_position.xyz;
	data.uvs = uvs;
	data.normal = normal;
}