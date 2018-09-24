#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 texture_coords;
out vec3 pass_world_position;
out vec3 pass_normal;
out vec2 pass_texture_coords;
out vec4 shadow;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 shadow_bias;

void main(void)
{
	pass_world_position = vec3(model_matrix * vec4(vertex_position, 1.0f));

	gl_Position = projection_matrix * view_matrix * vec4(pass_world_position, 1.0f);

	pass_normal = vec3(model_matrix * vec4(vertex_normal, 0.0f));

	pass_texture_coords = texture_coords;

	shadow = shadow_bias * vec4(pass_world_position, 1.0f);
}