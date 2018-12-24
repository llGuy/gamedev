#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 uvs;

uniform mat4 projection_matrix;
uniform mat4 view_matrix_no_translation;
uniform mat4 model_matrix;

out VS_DATA {
	vec2 uvs;
} vs_out;

void main(void)
{
	gl_Position = projection_matrix * view_matrix_no_translation * model_matrix * vec4(vertex_position, 1.0);

	vs_out.uvs = uvs;
}