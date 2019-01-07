#version 400

layout(location = 0) out vec4 final_color;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gExtra;

in VS_DATA{
	vec2 uvs;
} fs_in;

layout(std140) uniform shadow_data
{
	mat4 shadow_bias;

	float transition_distance;
	float shadow_distance;
	float map_size;
	float pcf_count;
}
shadow_info;

layout(std140, row_major) uniform light
{
	vec4 light_position;
	vec4 ambient_intensity;
	vec4 diffuse_intensity;
	vec4 specular_intensity;
}
light_info;

uniform vec3 camera_position;

const float PI = 3.14159265359;

float normal_distribution_ggx(vec3 vs_normal, vec3 halfway, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float ndoth = max(dot(vs_normal, halfway), 0.0);
	float a2minus1 = a2 - 1;
	float integral_den = ndoth * ndoth * a2minus1 + 1;

	float num = a2;
	float den = PI * integral_den * integral_den;

	return num / den;
}

float geometry_schlick_ggx(float ndotv, float roughness)
{
	float r = (roughness + 1);
	float k = (r * r) / 8;

	float num = ndotv;
	float den = ndotv * (1 - k) + k;

	return num / den;
}

float geometry_smith(vec3 n, vec3 v, vec3 l, float roughness)
{
	float ndotv = max(dot(n, v), 0.0);
	float ndotl = max(dot(n, l), 0.0);

	return geometry_schlick_ggx(ndotv, roughness) * geometry_schlick_ggx(ndotl, roughness);
}

vec3 fresnel_schlick(float cos_theta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cos_theta, 5.0);
}

vec4 pbr(void)
{
	vec4 albedo = texture(gAlbedo, fs_in.uvs);
	albedo.xyz = pow(albedo.xyz, vec3(2.2));
	vec3 vs_position = texture(gPosition, fs_in.uvs).xyz;
	vec4 gnormal = texture(gNormal, fs_in.uvs);
	vec3 vs_normal = gnormal.xyz;
	float roughness = albedo.a;
	float metallic = gnormal.a;

	vec3 radiance = vec3(24.47, 21.31, 20.79);
	vec3 light_vector = normalize(vec3(light_info.light_position));
	vec3 to_camera = normalize(-vs_position);
	vec3 halfway = normalize(to_camera + light_vector);

	float n_dot_l = max(dot(vs_normal, light_vector), 0.0);

	// BRDF

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo.xyz, metallic);
	vec3 F = fresnel_schlick(max(dot(vs_normal, to_camera), 0.0), F0);
	float D = normal_distribution_ggx(vs_normal, halfway, roughness);
	float G = geometry_smith(vs_normal, to_camera, light_vector, roughness);

	vec3 num = F * D * G;
	float den = 4 * max(dot(vs_normal, to_camera), 0.0) * max(dot(vs_normal, light_vector), 0.0);
	vec3 spec = num / max(den, 0.001);

	vec3 ks = F * 0.5;
	vec3 kd = vec3(1.0) - ks;
	kd *= 1.0 - metallic;

	vec3 result = (kd * vec3(albedo) / PI + spec) * radiance * n_dot_l;

	vec3 ambient = vec3(0.03) * vec3(albedo);
	result += ambient;

	result = result / (result + vec3(1.0));
	result = pow(result, vec3(1.0 / 2.2));

	return vec4(result, 1.0);
}

void main(void)
{
	vec4 in_color = texture(gAlbedo, fs_in.uvs);
	if (in_color.a > 0.000001)
	{
		/*vec4 extra = texture(gExtra, fs_in.uvs);

		vec3 vs_position = texture(gPosition, fs_in.uvs).xyz;
		vec3 vs_normal = texture(gNormal, fs_in.uvs).xyz;

		vec3 light_vector = normalize(-light_info.light_position).xyz;


		float diffuse_value = clamp(dot(vs_normal, -light_vector), 0, 1);


		vec3 to_camera = normalize(-vs_position);
		vec3 reflected_light = reflect(light_vector, vs_normal);
		float specular = clamp(dot(reflected_light, to_camera), 0, 1);
		specular = pow(specular, 10);

		final_color = in_color + vec4(diffuse_value) + vec4(specular);*/
		//final_color = in_color;

		final_color = pbr();
	}
	else
	{
		final_color = in_color;
	}
}