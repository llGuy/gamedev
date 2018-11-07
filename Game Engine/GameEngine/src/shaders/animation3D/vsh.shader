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

const int upper_arm_left = 4;
const int lower_arm_left = 5; 
const int upper_arm_right = 7;
const int lower_arm_right = 8;
const int hand_right = 9;
const int hand_left = 6;

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
		if (joint_ids[i] == lower_arm_left || joint_ids[i] == upper_arm_left || joint_ids[i] == hand_left)
		{
			/*joint = mat4(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				joint[3][0], joint[3][1], joint[3][2], 1
			);
			original_pos.w = 0.0f;*/
		}
		
		vec4 pose_position = joint * original_pos;

		accumulated_local += pose_position * weights[i];

		vec4 world_normal = joint * vec4(vertex_normal, 0.0f);
		accumulated_normal += world_normal * weights[i];
	}
	
    vertex_out.vertex_position = vec3(model_matrix * accumulated_local);

	//vertex_out.vertex_position = vec3(joint_transforms[joint_ids[0]] * vec4(vertex_position, 1.0f));
	//mat4 t = joint_transforms[joint_ids[0]];
	//vec3 translation = vec3(t[3][0], t[3][1], t[3][2]);
	//vertex_out.vertex_position = vertex_position + translation * weights[0];
	//vertex_out.vertex_position = vec3(model_matrix * vec4(vertex_position, 1.0f));
	vertex_out.texture_coords = vertex_uvs;
	vertex_out.vertex_normal = vec3(accumulated_normal);
	vertex_out.weights = weights;

	gl_Position = projection_matrix * view_matrix * vec4(vertex_out.vertex_position, 1.0f);
}