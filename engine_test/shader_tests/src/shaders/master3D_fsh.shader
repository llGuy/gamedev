layout(location = 0) out vec4 final_color;

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

uniform struct material
{
	vec3 ambient_reflectivity;
	vec3 diffuse_reflectivity;
	vec3 specular_reflectivity;
	float shininess_factor;
} 
material_info;

uniform struct light
{
	vec3 light_position;
	vec3 ambient_intensity;
	vec3 diffuse_intensity;
	vec3 specular_intensity;
}
light_info;

uniform sampler2D diffuse;
uniform vec3 light_position;
uniform vec3 camera_position;

uniform mat4 model_matrix;
uniform mat4 view_matrix;

void main(void)
{
#ifdef LINKED_TO_GSH
	input_prev input_data = geometry_out;
#else
	input_prev input_data = vertex_out;
#endif

#ifdef USES_TEXTURE
	final_color = texture(diffuse, input_data.texture_coords);
#endif

#ifdef USES_COLOR
	final_color = vec4(input_data.vertex_color, 1.0f);
#endif


	
}