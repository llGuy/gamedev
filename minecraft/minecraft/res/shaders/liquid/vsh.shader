#version 330 core

layout(location = 0) in vec3 vertex_position;

out vec3 pass_world_position;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform vec3 light_position;
uniform vec3 eye_position;

void main()
{
	pass_world_position = vertex_position;

	gl_Position = vec4(vertex_position, 1.0f);
}