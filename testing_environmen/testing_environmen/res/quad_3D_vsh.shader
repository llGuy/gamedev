#version 330 core

layout(location = 0) in vec3 vertex_position;
out vec3 pass_world_position;
out vec4 shadow_coord;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform vec3 translation;
uniform mat4 shadow_bias;
uniform vec3 camera_pos;
uniform vec3 color;
uniform mat4 model_matrix;

void main(void)
{
	pass_world_position = vec3(model_matrix * vec4(vertex_position, 1.0f));

	gl_Position = projection_matrix * view_matrix * vec4(pass_world_position, 1.0f);

	shadow_coord = shadow_bias * vec4(pass_world_position, 1.0f);

}