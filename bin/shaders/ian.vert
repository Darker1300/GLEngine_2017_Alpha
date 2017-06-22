#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec2 uv;
layout(location=2) in vec4 normal;
layout(location=3) in vec4 tangent;
layout(location=4) in vec4 bitangent;

uniform mat4 projectionViewMatrix;
uniform mat4 modelMatrix;

out vec2 vUV;

void main()
{
	vUV = uv;
	mat4 PVM = projectionViewMatrix * modelMatrix;
	gl_Position = PVM * position;
}
