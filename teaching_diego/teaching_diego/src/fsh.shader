#version 330 core

// takes in a vec3 (the vertex color)
in vec3 pass_vertex_color;

// the output of the fragment shader that will be outputted
// to the screen
out vec4 final_fragment_color;

void main(void)
{
	final_fragment_color = vec4(pass_vertex_color.r, pass_vertex_color.g, pass_vertex_color.b, 1.0f /* alpha */);
}