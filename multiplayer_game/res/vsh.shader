#version 320 es

in layout(location = 0) vec2 vertex_position;

/*uniform vec3 model_color;
uniform mat4 matrix_model;
uniform mat4 matrix_view;
uniform mat4 matrix_projection;*/

void main(void)
{
	gl_Position = vec4(vertex_position, 0.0f, 1.0f);
}
