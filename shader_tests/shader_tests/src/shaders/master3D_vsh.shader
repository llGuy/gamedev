#version 400

/* all possible attributes */
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 texture_coords;
layout(location = 3) in vec3 vertex_normal;

out struct
{
	vec3 vertex_position;

	vec3 vertex_color;

	vec3 texture_coords;

	vec3 vertex_normal;
} 
vertex_out;

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

void main(void)
{
	vertex_out.vertex_position = vec3(model_matrix * vec4(vertex_position, 1.0f));

#ifdef USES_COLOR
	vertex_out.vertex_color = vertex_color;
#endif

#ifdef USES_TEXTURE
	vertex_out.texture_coords = texture_coords;
#endif

#ifdef USES_STATIC_NORMALS
	vertex_out.vertex_normal = vec3(model_matrix* vec4(vertex_normal, 0.0f));
#endif

	gl_Position = projection_matrix * view_matrix * vec4(vertex_out.vertex_position, 1.0f);
}