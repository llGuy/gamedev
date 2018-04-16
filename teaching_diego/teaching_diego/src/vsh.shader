#version 330 core

in layout(location = 0) vec2 vertex_position;
in layout(location = 1) vec3 vertex_color;

// new stuff

// the variable output that will be sent to the fragment shader
out vec3 pass_vertex_color;

void main(void)
{
	// the output position of the vertex shader
	// gl_Position is an in-built variable in glsl
	// it means the output position of the vertex shader
	// it takes a vec4 (x, y, z, w) w is a thing for translating the vertices
	// i'll explain another day

	gl_Position = /* cast to a vec4 */ vec4(vertex_position.x, vertex_position.y, 0.0f, 1.0f);
	
	// when the main body of the vertex shader exits
	// the pass_vertex_color that will be sent to the fragment shader will be equal
	// to the vertex_color passed by attribute 1
	pass_vertex_color = vertex_color;
}