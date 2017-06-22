#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec2 uv;
layout(location=2) in vec4 normal;
layout(location=3) in vec4 tangent;
layout(location=4) in vec4 bitangent;

uniform mat4 projectionViewMatrix;
uniform mat4 modelMatrix;

out vec4 vPosition;
out vec2 vUV;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBiTangent;

void main()
{
	mat4 PVM = projectionViewMatrix * modelMatrix;

	vPosition = PVM * position;
	vUV = uv;
	vNormal = normal.xyz;
	vTangent = tangent.xyz;
	vBiTangent = bitangent.xyz;

	gl_Position = vPosition;
}
