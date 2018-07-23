#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 texture_coords;
out vec2 pass_texture_coords;
out vec3 light_vector;
out vec3 to_camera;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform vec3 light_position;
uniform vec3 camera_position;

void main(void)
{
	vec3 world_position = (model_matrix * vec4(vertex_position, 1.0f)).xyz;
	gl_Position = projection_matrix * view_matrix * vec4(world_position, 1.0f);
	pass_texture_coords = texture_coords;
	light_vector = world_position - light_position;
	to_camera = camera_position - world_position;
}