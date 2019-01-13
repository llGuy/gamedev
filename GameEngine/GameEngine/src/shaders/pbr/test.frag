#version 400

layout(location = 0) out vec4 gAlbedo;
layout(location = 2) out vec4 gPosition;
layout(location = 3) out vec4 gNormal;
layout(location = 4) out vec4 sun_only;

uniform vec3 color;
uniform float roughness;
uniform float metallic;

in VS_DATA {
    vec4 vs_position; 
    vec4 vs_normal;
    vec2 uvs;
} fs_in;

void main(void)
{
     gAlbedo = vec4(color, roughness);

     gPosition = fs_in.vs_position;

     gNormal = normalize(fs_in.vs_normal);
	 gNormal.a = metallic;

	 sun_only = vec4(0.0, 0.0, 0.0, 1.0);
}