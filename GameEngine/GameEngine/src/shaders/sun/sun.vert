#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 uvs;

layout(std140) uniform camera
{
	vec4 position;
	mat4 view_matrix;
	mat4 projection_matrix;
} 
camera_info;

uniform mat4 model_matrix;

out VS_DATA {
	vec2 uvs;
} vs_out;

void main(void)
{
	mat4 view_matrix_no_translation = camera_info.view_matrix;
	mat4 model_matrix_transpose_rotation = model_matrix;


	mat3 rotation_part = mat3(view_matrix_no_translation);
	rotation_part = transpose(rotation_part);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			model_matrix_transpose_rotation[i][j] = rotation_part[i][j];
		}
	}

	view_matrix_no_translation[3][0] = 0;
	view_matrix_no_translation[3][1] = 0;
	view_matrix_no_translation[3][2] = 0;

	gl_Position = camera_info.projection_matrix * view_matrix_no_translation * model_matrix_transpose_rotation * vec4(vertex_position * 50, 1.0);

	vs_out.uvs = uvs;
}