#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uvs;
layout(location = 2) in vec3 vertex_normal;

layout(std140) uniform camera
{
	vec4 position;
	mat4 view_matrix;
	mat4 projection_matrix;
} 
camera_info;

uniform mat4 model_matrix;

out VS_DATA {
    vec4 vs_position; 
    vec4 vs_normal;
    vec2 uvs;
} vs_out;

void main(void) {
     vs_out.vs_position = camera_info.view_matrix * model_matrix * vec4(vertex_position, 1.0);
	 vs_out.vs_normal =	camera_info.view_matrix * model_matrix * vec4(vertex_normal, 0.0);
     vs_out.uvs = vertex_uvs;

     gl_Position = camera_info.projection_matrix * vs_out.vs_position;
}