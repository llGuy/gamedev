#version 430

in layout(location = 0) vec3 aM_vertexPosition;
in layout(location = 1) vec3 aM_vertexColor;

uniform mat4 u_mvp;						//model view projection matrix

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;
uniform float u_lavaHeightTop;
uniform float u_time;

out vec3 pass_colour;
out vec3 pass_worldPosition;

float PI = 3.141592635897;
float amplitude = 0.1f;

float CalculateHeight(void)
{
	if (aM_vertexPosition.y > u_lavaHeightTop)
	{
		float component1 = sin(2.0 * PI * u_time / 2 + (aM_vertexPosition.x * 16.0f)) * amplitude;
		float component2 = sin(2.0 * PI * u_time / 2 + (aM_vertexPosition.z * aM_vertexPosition.x * 8.0f)) * amplitude;
		return component1 + component2 + aM_vertexPosition.y;
	}
	else return aM_vertexPosition.y;
}

void main()
{
	vec4 world_pos = u_modelMatrix * vec4(aM_vertexPosition.x, CalculateHeight(), aM_vertexPosition.z, 1.0);
	pass_worldPosition = vec3(world_pos.x, world_pos.y, world_pos.z);
	vec4 view_pos = u_viewMatrix * world_pos;
	gl_Position = u_projectionMatrix * view_pos;
	pass_colour = aM_vertexColor;
}