#version 400

layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 vertex_uvs;

out VS_DATA {

	vec2 uvs;

} vs_out;

void main(void)
{
	gl_Position = vec4(vertex_position, 0.0, 1.0);

	vs_out.uvs = vertex_uvs;
}