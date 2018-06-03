#version 330

layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 texture_coordinate;

out vec2 pass_texture_coordinate;

uniform mat4 matrix_projection;
uniform vec2 translation;
uniform float scale;

void main(void)
{
	//vec2 newPos = (vertex_position * scale) + translation;
	//vec4 projected_position = matrix_projection * vec4(newPos, 0.0f, 1.0f);
	//gl_Position = vec4(projected_position.xy, 0.0f, 1.0f);
	vec2 position;
	position.x = translation.x * 2.0 - 1.0;
	position.y = translation.y * -2.0 + 1.0;
	gl_Position = vec4(position, 0.0, 1.0);

	pass_texture_coordinate = texture_coordinate;
}