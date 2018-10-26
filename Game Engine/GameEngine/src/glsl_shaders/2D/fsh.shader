layout(location = 0) out vec4 final_color;

in VS_OUT
{

	vec2 vertex_position;
	vec2 uv;
	vec4 color;
	int texture_id;

} fs_in;

uniform sampler2D textures[32];

void main(void)
{
	if (fs_in.texture_id > 0)
	{
		if (fs_in.color.a > -0.001f)
		{
			final_color = texture(textures[fs_in.texture_id], fs_in.uv);
		}
		else
		{
			final_color = fs_in.color;
			final_color.a = texture(textures[fs_in.texture_id], fs_in.uv).a;
		}
	}
	else
	{
		final_color = fs_in.color;
	}
}