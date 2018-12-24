#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uvs;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec3 vertex_tangent;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

out VS_DATA {
	vec3 w_position;
	vec3 v_position;
	vec3 w_normal;
	vec3 v_normal;
	vec2 uvs;
} vs_out;

void main(void)
{
	vs_out.w_position = vec3(model_matrix * vec4(vertex_position, 1.0));
	vs_out.v_position = vec3(view_matrix * vec4(vs_out.w_position, 1.0));
	vs_out.w_normal = mat3(model_matrix) * vertex_normal;
	vs_out.v_normal = mat3(view_matrix) * vs_out.w_normal;
	vs_out.uvs = vertex_uvs;

//	mat3 tangent_space (
//		tangent.x, bitangent_x, normal.x
//	);

	gl_Position = projection_matrix * vec4(vs_out.v_position);
}