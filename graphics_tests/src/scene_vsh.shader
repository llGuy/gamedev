#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_coords;
out vec2 pass_texture_coords;
out vec3 pass_world_positions;
out vec4 pass_shadow_coord;

//const vec4 plane = vec4(0, -1, 0, 15);

uniform vec4 plane;
uniform vec3 model_color;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat4 depth_bias;

void main(void)
{
	gl_Position = projection_matrix * view_matrix * vec4(vertex_position, 1.0f);
	pass_texture_coords = texture_coords;

	pass_world_positions = vertex_position;
	pass_shadow_coord = depth_bias * vec4(vertex_position, 1.0f);

	gl_ClipDistance[0] = dot(vec4(pass_world_positions, 1.0f), plane);
}