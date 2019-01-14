#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uvs;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec3 vertex_tangent;

out VS_OUT {
	vec3 position;
	vec2 uvs;
	vec3 normal;
	vec4 shadow_coord;
	vec4 view_position;
	vec3 to_light;
	vec3 to_camera;
}
vs_out;

uniform mat4 model_matrix;

layout(std140) uniform camera
{
	vec4 position;
	mat4 view_matrix;
	mat4 projection_matrix;
} 
camera_info;

layout(std140, row_major) uniform light
{
    vec4 light_position;
	vec4 ambient_intensity;
	vec4 diffuse_intensity;
	vec4 specular_intensity;
} 
light_info;

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
	vs_out.position = vec3(model_matrix * vec4(vertex_position, 1.0));

	vs_out.shadow_coord = shadow_info.shadow_bias * vec4(vs_out.position, 1.0);

	vs_out.uvs = vertex_position.xz * 20;

	vs_out.normal = vec3(camera_info.view_matrix * model_matrix * vec4(vertex_normal, 0.0));

	vs_out.view_position = camera_info.view_matrix * vec4(vs_out.position, 1.0);

	vec4 vs_light_pos = camera_info.view_matrix * light_info.light_position;

	vs_out.to_light = (-vs_light_pos.xyz);

	vs_out.to_camera = vec3(vs_out.view_position);


	gl_Position = camera_info.projection_matrix * vs_out.view_position;
}