layout(location = 0) in vec3 vertex_position;

out vec3 direction;
out vec4 view_position;

uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 view_matrix_no_translation;

void main(void)
{
	view_position = view_matrix_no_translation * model_matrix * vec4(vertex_position, 1);

	gl_Position = projection_matrix * view_position;

	direction = vertex_position;
}