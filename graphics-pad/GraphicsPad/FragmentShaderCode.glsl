#version 430

in vec3 normalWorld;
in vec3 vertexPositionWorld;

out vec4 finalFragmentColor;

uniform vec4 ambientLight;
uniform vec3 eyePositionWorld;
uniform vec3 lightPositionWorld;

void main()
{
	vec3 lightVector = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = dot(lightVector, normalize(normalWorld));
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);

	vec3 reflectedLightWorld = reflect(-lightVector, normalWorld);
	vec3 eyeVector = normalize(eyePositionWorld - vertexPositionWorld);
	float specularity = dot(reflectedLightWorld, eyeVector);
	//specularity = pow(specularity, 3);
	specularity = specularity * specularity * specularity;
	vec4 specularLight = vec4(specularity, specularity, specularity, 1);
	finalFragmentColor = clamp(diffuseLight, 0, 1) + ambientLight + clamp(specularLight, 0, 1);
}