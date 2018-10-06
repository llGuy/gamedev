#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 texture_coords;
layout(location = 3) in mat4 transform_matrix;

out vec3 world_position;
out vec3 normal;
out vec2 otexture_coords;
out vec4 pass_shadow;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 shadow_bias;

void main(void)
{
	world_position = vec3(transform_matrix * vec4(vertex_position, 1.0f));

	gl_Position = projection_matrix * view_matrix * vec4(world_position, 1.0f);

	normal = vec3(model_matrix * vec4(vertex_normal, 0.0f));

	otexture_coords = texture_coords;

	pass_shadow = shadow_bias * vec4(world_position, 1.0f);
}