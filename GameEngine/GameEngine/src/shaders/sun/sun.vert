#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 uvs;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

out VS_DATA {
	vec2 uvs;
} vs_out;

void main(void)
{
	mat4 view_matrix_no_rotation = view_matrix;

	view_matrix_no_rotation[0][0] = 1;
	view_matrix_no_rotation[0][1] = 0;
	view_matrix_no_rotation[0][2] = 0;

	view_matrix_no_rotation[1][0] = 0;
	view_matrix_no_rotation[1][1] = 1;
	view_matrix_no_rotation[1][2] = 0;

	view_matrix_no_rotation[2][0] = 0;
	view_matrix_no_rotation[2][1] = 0;
	view_matrix_no_rotation[2][2] = 1;

	gl_Position = projection_matrix * view_matrix_no_rotation * model_matrix * vec4(vertex_position, 1.0);

	vs_out.uvs = uvs;
}