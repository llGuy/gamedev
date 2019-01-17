#version 400

layout(location = 0) out vec4 gAlbedo;
layout(location = 2) out vec4 gPosition;
layout(location = 3) out vec4 gNormal;
layout(location = 4) out vec4 sun_only;

in VS_DATA {

	vec3 color;
	vec2 uvs;

} fs_in;

uniform sampler2D heightmap;
uniform sampler2D diffuse;

void main(void)
{
	 gAlbedo = vec4(fs_in.color, 0.4);

	 gAlbedo = texture(heightmap, fs_in.uvs);
	 
//	 gPosition = fs_in.vs_position;

	 gPosition.a = -1;

     gNormal = vec4(10.0);

	 sun_only = vec4(0.0, 0.0, 0.0, 1.0);
}