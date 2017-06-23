#version 410

in vec3 vColour;

out vec4 fragColour;

void main()
{
	fragColour = vec4(vColour, 1);
}
