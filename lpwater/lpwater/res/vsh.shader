#version 430

in layout(location = 0) vec3 vertexPosition;
in layout(location = 1) vec3 vertexColor;

out vec3 color;
out vec3 normal;
out vec3 position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

const vec4 plane = vec4(0, -1, 0, 15);

void main(void)
{
	vec4 worldPos = model * vec4(vertexPosition, 1.0f);
	gl_ClipDistance[0] = dot(worldPos, plane);
	position = worldPos.xyz;
	vec4 viewPosition = view * vec4(position, 1.0f);
	gl_Position = projection * viewPosition;
	color = vertexColor;
}