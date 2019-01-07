layout(location = 0) out vec4 final_color;

in struct input_prev
{
	vec2 texture_coords;
}
vertex_out;

uniform sampler2D scene_depth;
uniform sampler2D diffuse;

uniform mat4 inverse_proj_matrix;
uniform mat4 inverse_view_matrix;
uniform mat4 previous_view_proj;
uniform float current_fps;

const float near = 0.1f;
const float far = 10000.0f;

float linearize_depth(float depth)
{
	return 2.0f * near * far / (far + near - (2 * depth - 1) * (far - near));
}

void main(void)
{
	float current_z = texture(scene_depth, vertex_out.texture_coords).x * 2 - 1;
	float current_x = vertex_out.texture_coords.x * 2 - 1;
	float current_y = vertex_out.texture_coords.y * 2 - 1;

	vec4 projection_pos = vec4(current_x, current_y, current_z, 1);

	vec4 position_view_space = inverse_proj_matrix * projection_pos;

	position_view_space /= position_view_space.w;

	vec4 world_space = inverse_view_matrix * position_view_space;

	vec4 previous = previous_view_proj * world_space;
	previous.xyz /= previous.w;
	previous.xy = previous.xy * 0.5 + 0.5;

	vec2 blur_vector = (vertex_out.texture_coords - previous.xy);

	float target_fps = 40;
	blur_vector = (previous.xy - vertex_out.texture_coords) * (current_fps / target_fps);

	vec4 result = texture(diffuse, vertex_out.texture_coords);
	int num_samples = 3;
	for (int i = 1; i < num_samples; ++i)
	{
		vec2 offset = blur_vector * (float(i) / float(num_samples - 1) - 0.5);
		vec2 uvs = offset + vertex_out.texture_coords;
		if (uvs.x > 1.0 || uvs.y > 1.0 || uvs.x < 0.0 || uvs.y < 0.0)
			result += texture(diffuse, vertex_out.texture_coords);
		else result += texture(diffuse, vertex_out.texture_coords + offset);
	}
	result /= float(num_samples);

	final_color = result;
	final_color.a = 1;
}