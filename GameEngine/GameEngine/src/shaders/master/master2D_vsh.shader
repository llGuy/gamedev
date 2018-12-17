layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 texture_coords;
layout(location = 2) in float texture_slot;

out struct
{
	vec2 texture_coords;

	float texture_slot;
} 
vertex_out;

uniform mat4 projection_matrix;

void main(void)
{
	gl_Position = projection_matrix * vec4(vertex_position, 0.0f, 1.0f);

	vertex_out.texture_coords = texture_coords;
	vertex_out.texture_slot = texture_slot;
}