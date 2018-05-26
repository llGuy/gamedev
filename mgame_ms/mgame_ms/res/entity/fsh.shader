#version 320 es

precision highp float;

//in vec3 pass_world_position;

out vec4 final_color;

//uniform vec3 light_position;
uniform vec3 model_color;

//const vec3 NORMAL = vec3(0.0f, 1.0f, 0.0f);

void main(void)
{
	// calculate diffuse
//	vec3 light_vector = light_position - pass_world_position;
//	float brightness = dot(light_vector, NORMAL) * 0.5f;
	
	final_color = vec4(model_color, 1.0f);
}
