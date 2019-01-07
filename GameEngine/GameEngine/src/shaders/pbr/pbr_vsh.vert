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
	vec3 tangent;
	vec3 bitangent;
	vec4 view_position;
	vec4 tangent_view_position;
	vec3 to_light;
	vec3 to_camera;
	vec3 tangent_camera_pos;

	vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
	
	mat3 inv_tangent;
	mat3 to_tangent_space;
	mat3 ws_inv_tangent;
}
vs_out;

uniform mat4 model_matrix;

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

layout(std140) uniform camera
{
	vec4 position;
	mat4 view_matrix;
	mat4 projection_matrix;
} 
camera_info;

void main(void)
{
	vs_out.position = vec3(model_matrix * vec4(vertex_position, 1.0));

	vs_out.shadow_coord = shadow_info.shadow_bias * vec4(vs_out.position, 1.0);

	vs_out.uvs = vertex_uvs;

	vs_out.tangent = vertex_tangent;

	vs_out.normal = vec3(camera_info.view_matrix * model_matrix * vec4(vertex_normal, 0.0));

	vs_out.view_position = camera_info.view_matrix * vec4(vs_out.position, 1.0);

	vec3 ws_norm = normalize(mat3(model_matrix) * vertex_normal);
	vec3 ws_tang = normalize((model_matrix * vec4(vertex_tangent, 0.0)).xyz);
	vec3 ws_bitang = normalize(cross(ws_norm, ws_tang));

	mat3 to_tangent_ws = mat3 (
		ws_tang.x, ws_bitang.x, ws_norm.x,
		ws_tang.y, ws_bitang.y, ws_norm.y,
		ws_tang.z, ws_bitang.z, ws_norm.z
	);

	vs_out.ws_inv_tangent = inverse(to_tangent_ws);
   
	vec3 norm = normalize(vs_out.normal);
	vec3 tang = normalize((camera_info.view_matrix * model_matrix * vec4(vertex_tangent, 0.0)).xyz);
	vec3 bitang = normalize(cross(norm, tang));

	mat3 to_tangent_space = mat3(
		tang.x, bitang.x, norm.x,
		tang.y, bitang.y, norm.y,
		tang.z, bitang.z, norm.z
	);

	vs_out.tangent_view_position = vec4(to_tangent_space * vs_out.view_position.xyz, 1.0);

	vec4 vs_light_pos = camera_info.view_matrix * light_info.light_position;

	vs_out.to_light = to_tangent_space * (-vs_light_pos.xyz);

	vs_out.to_camera = to_tangent_space * vec3(vs_out.view_position);

	vs_out.tangent_camera_pos = to_tangent_space * vec3(0);

	vs_out.to_tangent_space = to_tangent_space;
	vs_out.inv_tangent = inverse(to_tangent_space);

	vs_out.TangentLightPos = to_tangent_space * vec3(light_info.light_position);
	vs_out.TangentViewPos = to_tangent_space * vec3(camera_info.position);
	vs_out.TangentFragPos = to_tangent_space * vs_out.position;



	gl_Position = camera_info.projection_matrix * vs_out.view_position;
}