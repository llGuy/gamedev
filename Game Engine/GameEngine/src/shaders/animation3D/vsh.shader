#define MAX_JOINTS 50
#define MAX_WEIGHTS 3

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uvs;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in ivec3 joint_ids;
layout(location = 4) in vec3 weights;

out struct input_prev
{
	vec3 vertex_position;

	vec2 texture_coords;

	vec3 vertex_normal;

	vec3 weights;
}
vertex_out;

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

uniform mat4 joint_transforms[MAX_JOINTS];

void main(void)
{
#ifdef USES_TEXTURE
	vertex_out.texture_coords = vertex_uvs;
#endif

	mat4 identity = mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);

	vec4 accumulated_local = vec4(0);
	vec4 accumulated_normal = vec4(0);

	for (int i = 0; i < MAX_WEIGHTS; ++i)
	{
		vec4 original_pos = vec4(vertex_position, 1.0f);

		mat4 joint = joint_transforms[joint_ids[i]];
		
		vec4 pose_position = joint * original_pos;

		accumulated_local += pose_position * weights[i];

		vec4 world_normal = joint * vec4(vertex_normal, 0.0f);
		accumulated_normal += world_normal * weights[i];
	}
	
    vertex_out.vertex_position = vec3(model_matrix * accumulated_local);

	vertex_out.texture_coords = vertex_uvs;
	vertex_out.vertex_normal = vec3(accumulated_normal);
	vertex_out.weights = weights;

	gl_Position = projection_matrix * view_matrix * vec4(vertex_out.vertex_position, 1.0f);
}