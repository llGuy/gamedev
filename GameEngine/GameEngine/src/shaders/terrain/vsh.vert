#version 400

layout(location = 0) in vec2 vertex_position;

layout(std140) uniform camera
{
	vec4 position;
	mat4 view_matrix;
	mat4 projection_matrix;
} 
camera_info;

out VS_DATA {

	vec3 color;
	vec2 uvs;

} vs_out;

uniform int size;
uniform mat4 model_matrix;
uniform sampler2D height_map;

void main(void)
{
//	vec2 uvs = vertex_position / float(size) + vec2(1.0);

	vec3 translate = vec3(model_matrix[3][0], model_matrix[3][1], model_matrix[3][2]);
	vec3 scale = vec3(model_matrix[0][0], model_matrix[1][1], model_matrix[2][2]);

	vec3 cam_translate_orig = vec3(camera_info.position.x, 0.0, camera_info.position.z);

	/* controls how often the terrain translates */ 
	//vec3 cam_translate = floor(cam_translate_orig / vec3(scale.x, 1.0, scale.z)) * scale;
	//cam_translate -= mod(cam_translate, vec3(2) / vec3(scale.x, 1.0, scale.z));
	vec3 cam_translate = roundEven(cam_translate_orig / (vec3(2) * scale)) * vec3(2);

	vec2 uvs = (translate + cam_translate + vec3(vertex_position.x, 0.0, vertex_position.y)).xz / size;
	float height = texture(height_map, uvs).x * 30;

	vec4 world_pos = model_matrix * vec4(vertex_position.x, height, vertex_position.y, 1.0);
	world_pos.xz += cam_translate.xz;

	gl_Position = camera_info.projection_matrix * camera_info.view_matrix * world_pos;

	vs_out.color = vec3(0.0);
	vs_out.uvs = uvs;
}