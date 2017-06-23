#version 410

uniform vec3 ambientMat = vec3(0,0,0);

out vec4 fragColour;

void main()
{
	fragColour = vec4(ambientMat, 1.0f);
}
