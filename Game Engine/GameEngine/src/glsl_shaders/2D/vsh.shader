layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 color;
layout(location = 3) in int texture_id;

out VS_OUT
{

	vec2 vertex_position;
    vec2 uv;
	vec4 color;
	int texture_id;

} fs_in;

void main(void)
{
	gl_Position = vec4(vertex_position, 1.0f);

	fs_in.vertex_position = vertex_position;
	fs_in.uv = uv;
	fs_in.color = color;
	fs_in.texture_id = texture_id;
}