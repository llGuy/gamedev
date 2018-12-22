#version 400

layout(location = 0) out vec4 final_color;
layout(location = 2) out vec4 view_position;
layout(location = 3) out vec4 view_normal;

uniform sampler2D diffuse_map;
uniform sampler2D normal_map;
uniform mat4 view_matrix;

in struct input_prev
{
	vec3 position;

	vec2 uvs;

	vec3 normal;

	vec4 shadow_coord;

	vec3 tangent;

	vec3 bitangent;

	vec4 view_position;
} 
vertex_out;

void main(void)
{
	vec4 color = texture(diffuse_map, vertex_out.uvs);
	vec4 normal = texture(normal_map, vertex_out.uvs);
	final_color = color;

	view_normal = view_matrix * vec4(vertex_out.normal, 1.0);

	view_position = vertex_out.view_position;
}