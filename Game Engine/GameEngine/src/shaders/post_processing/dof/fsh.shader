layout(location = 0) out vec4 final_color;

in struct input_prev
{
	vec2 texture_coords;
}
vertex_out;

uniform sampler2D diffuse;
uniform sampler2D blurred;
uniform sampler2D depth_map;

const float near = 0.001f;
const float far = 10000.0f;

float convert_depth(float d)
{
	float depth = 2.0f * near * far / (far + near - (2.0f * d - 1.0f) * (far - near));
	return final_color = vec4(depth);
}

void main(void)
{
	float depth_at_center = convert_depth(texture(depth_map, vec2(0.5f)).r);

	float depth_at_point = convert_depth(texture(depth_map, vertex_out.texture_coords).r);

	float diff = clamp(abs(depth_at_center - depth_at_point), 0, 1.0f);

	final_color = mix(texture(diffuse, vertex_out.texture_coords), texture(blurred, vertex_out.texture_coords), diff);
}