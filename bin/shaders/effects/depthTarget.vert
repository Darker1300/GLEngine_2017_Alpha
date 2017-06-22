#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec2 uv;
layout(location=2) in vec4 normal;
layout(location=3) in vec4 tangent;
layout(location=4) in vec4 bitangent;

uniform mat4 projectionViewMatrix;
uniform mat4 modelMatrix;

out vec2 vUV;
out vec3 vPosition;

void main()
{
	vUV = uv;
	//mat4 PVM = projectionViewMatrix * modelMatrix;
	vec4 pos = position;
	// pos.z += 1;
	gl_Position = pos;// PVM * position;
	vPosition = gl_Position.xyz;
}
