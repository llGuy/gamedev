layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 texture_coords;

noperspective out struct input_prev
{
	vec2 texture_coords;
} 
vertex_out;

void main(void)
{
	gl_Position = vec4(vertex_position, 0, 1);

	vertex_out.texture_coords = texture_coords;
}