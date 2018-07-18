#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
out vec3 pass_vertex_color;
out vec3 pass_world_positions;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main(void)
{
	gl_ClipDistance[0] = -1;

	vec4 world_position = model_matrix * vec4(vertex_position, 1.0f);
	gl_Position = projection_matrix * view_matrix * world_position;

	pass_world_positions = vec3(world_position);
	pass_vertex_color = vertex_color;
}