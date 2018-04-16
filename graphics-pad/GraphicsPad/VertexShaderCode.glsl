#version 430

in layout(location=0) vec4 vertexPositionModel;
in layout(location=1) vec3 vertexColorModel;
in layout(location = 2) vec3 normalModel;

uniform vec3 lightPos;
uniform mat4 mvp;			// model view projection
uniform mat4 modelToWorldTransformMatrix;

out vec3 normalWorld;
out vec3 vertexPositionWorld;

void main()
{
	gl_Position = mvp * vertexPositionModel;
	//normalWorld = vec3(modelToWorldTransformMatrix * normalize(vertexPositionModel));
	normalWorld = vec3(modelToWorldTransformMatrix * vec4(normalModel, 0));
	vertexPositionWorld = vec3(modelToWorldTransformMatrix * vertexPositionModel);
}