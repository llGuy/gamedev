#version 330 core

layout(location = 0) in vec3 vertex_position;
out vec4 clip_space;
out vec2 texture_coords;
out vec3 to_camera;

uniform vec3 camera_position;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

const float tiling = 6.0f;

void main(void)
{
	gl_ClipDistance[0] = -1;
	clip_space = projection_matrix * view_matrix * vec4(vertex_position, 1.0f);;
	gl_Position = clip_space;

	texture_coords = (vec2(vertex_position.x / 2.0f + 0.5f, vertex_position.z / 2.0f + 0.5f) * 6.0f )/ 512.0f;
	to_camera = camera_position - vertex_position;
}