layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 texture_coords;
layout(location = 2) in vec3 vertex_color;

out struct
{
	vec3 vertex_color;

	vec2 texture_coords;
} 
vertex_out;

uniform mat4 projection_matrix;

void main(void)
{
	gl_Position = projection_matrix * vec4(vertex_position, 0.0f, 1.0f);

#ifdef USES_COLOR
	vertex_out.color = vertex_color;
	vertex_out.texture_coords = vec2(0.0f);
#endif

#ifdef USES_TEXTURE
	vertex_out.vertex_color = vec3(0.0f);
	vertex_out.texture_coords = texture_coords;
#endif
}