#version 400

const int MAX_JOINTS = 50;
const int MAX_WEIGHTS = 3;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in ivec3 joint_indices;
layout(location = 4) in vec3 weights;

out vec2 pass_uv;
out vec3 pass_normal;

uniform mat4 joint_transforms[MAX_JOINTS];
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

void main(void)
{
	vec4 total_local_pos = vec4(0.0f);
	vec4 total_normal = vec4(0.0f);

	for (int i = 0; i < MAX_WEIGHTS; ++i)
	{
		mat4 joint_transform = joint_transforms[joint_indices[i]];
		vec4 pose_position = joint_transform * vec4(vertex_position, 1.0f);
		total_local_pos += pose_position * weights[i];

		vec4 world_normal = joint_transform * vec4(vertex_normal, 0.0f);
		total_normal += world_normal * weights[i];
	}

	gl_Position = projection_matrix * view_matrix * total_local_pos;

	pass_normal = total_normal.xyz;

	pass_uv = vertex_uv;
}