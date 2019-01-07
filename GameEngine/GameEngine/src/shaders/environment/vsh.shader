layout(location = 0) in vec3 vertex_position;

out vec3 direction;
out vec4 view_position;

uniform mat4 model_matrix;

layout(std140) uniform camera
{
	vec4 position;
	mat4 view_matrix;
	mat4 projection_matrix;
} 
camera_info;

void main(void)
{
	mat4 view_no_translate = camera_info.view_matrix;
	view_no_translate[3][0] = 0.0;
	view_no_translate[3][1] = 0.0;
	view_no_translate[3][2] = 0.0;

	view_position = view_no_translate * model_matrix * vec4(vertex_position, 1.0);

	gl_Position = camera_info.projection_matrix * view_position;

	direction = vertex_position;
}