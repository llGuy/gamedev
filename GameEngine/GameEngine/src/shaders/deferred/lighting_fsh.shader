#define MAX_LIGHTS 10

layout(location = 0) out vec4 final_color;

in struct input_prev
{
	vec2 texture_coords;
}
vertex_out;

/* G-Buffer */
uniform sampler2D positions;
uniform sampler2D normals;
uniform sampler2D albedo;
uniform vec3 camera_position;

/* lights */
layout(std140, row_major) uniform lights
{
	int num_lights;
	vec3 light_positions[MAX_LIGHTS];
};

vec3 light_positionss[1] = { vec3(100, 100, 100) };

/* Calculate the diffuse */
void diffuse(inout vec4 diffuse_total, in vec3 vertex_normal, in vec3 light_vector)
{
	float diffuse = clamp(dot(light_vector, vertex_normal), 0, 1);

	diffuse_total.rgb += vec3(diffuse);
}

/* Calculate the specular highlights */
void specular(inout vec4 specular_total, in float specularity
	, in vec3 vertex_normal, in vec3 light_vector, in vec3 eye_vector)
{
	vec3 reflected_light = reflect(normalize(light_vector), vertex_normal);
	float specular = clamp(dot(reflected_light, normalize(eye_vector)), 0, 1);

	/* color.a is the specularity of the pixel */
	specular = pow(specular, specularity);

	specular_total += vec4(specular);
}

/* Need to add support for multiple lights - light attenuation equation - */

void main(void)
{
	final_color = texture(albedo, vertex_out.texture_coords);
	vec3 pixel_3D_position = texture(positions, vertex_out.texture_coords).xyz;
	vec4 pixel_3D_normal = texture(normals, vertex_out.texture_coords);

	//final_color = vec4(num_lights);

	vec4 total_diffuse = vec4(0);
	vec4 total_specular = vec4(0);

	for (int i = 0; i < 1; ++i)
	{
		vec3 light_vector = normalize(light_positionss[i] - pixel_3D_position);
		vec3 eye_vector = normalize(camera_position - pixel_3D_position);

		diffuse(total_diffuse, pixel_3D_normal.xyz, light_vector);

		specular(total_specular, pixel_3D_normal.a - 1.0f, pixel_3D_normal.xyz, light_vector, eye_vector);
	}
	final_color = total_diffuse;
//	final_color += total_diffuse + total_specular;
}