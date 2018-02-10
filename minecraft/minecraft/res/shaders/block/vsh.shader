#version 430

in layout(location = 0) vec3 vertex_position;
in layout(location = 1) vec3 texture_data;

out vec3 pass_world_position;
out vec3 pass_texture_data;
out float pass_visibility;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform vec3 light_position;
uniform vec3 eye_position;

const float density = 0.0071;
const float gradient = 1.5;

void main()
{
	pass_world_position = vertex_position;
	pass_texture_data = texture_data;
	
	gl_Position = vec4(vertex_position, 1.0f);
}