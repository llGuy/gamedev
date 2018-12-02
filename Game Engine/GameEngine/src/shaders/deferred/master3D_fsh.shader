layout(location = 0) out vec4 position;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 albedo;

/* determine whether is linked to gsh or not */
#ifdef LINKED_TO_GSH

in struct input_prev
{
	vec3 vertex_position;

	vec3 vertex_color;

	vec2 texture_coords;

	vec3 vertex_normal;
}
geometry_out;

#else

in struct input_prev
{
	vec3 vertex_position;

	vec3 vertex_color;

	vec2 texture_coords;

	vec3 vertex_normal;
} 
vertex_out;

#endif

layout(std140, row_major) uniform material
{
	vec4 ambient_reflectivity;
	vec4 diffuse_reflectivity;
	vec4 specular_reflectivity;
	float shininess_factor;
	float reflect_factor;
}
material_info;

uniform sampler2D diffuse;

void main(void)
{
#ifdef LINKED_TO_GSH
	input_prev input_data = geometry_out;
#else
	input_prev input_data = vertex_out;
#endif
#ifdef USES_TEXTURE
	albedo = texture(diffuse, input_data.texture_coords);
#endif

#ifdef USES_COLOR
	albedo = vec4(input_data.vertex_color, 1.0f);
#endif

	position = vec4(input_data.vertex_position, 1.0f);
	normal = vec4(input_data.vertex_normal, 1.0f);

	normal.a = material_info.reflect_factor + 1.0f;
}