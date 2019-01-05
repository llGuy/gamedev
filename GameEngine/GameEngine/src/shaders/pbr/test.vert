#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uvs;
layout(location = 2) in vec3 vertex_normal;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

out VS_DATA {
    vec4 vs_position; 
    vec4 vs_normal;
    vec2 uvs;
} vs_out;

void main(void) {
     vs_out.vs_position = view_matrix * model_matrix * vec4(vertex_position, 1.0);
	 vs_out.vs_normal =	view_matrix * model_matrix * vec4(vertex_normal, 0.0);
     vs_out.uvs = vertex_uvs;

     gl_Position = projection_matrix * vs_out.vs_position;
}