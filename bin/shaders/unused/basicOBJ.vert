#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;

uniform mat4 projectionViewMatrix;
uniform mat4 modelMatrix;

out vec3 vColour;

void main()
{
	vColour = max(normal, vec3(uv, sin(uv.x + uv.y * uv.y)));
	mat4 PVM = projectionViewMatrix * modelMatrix;
	gl_Position = PVM * position;
}
