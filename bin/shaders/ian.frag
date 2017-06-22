#version 410

in vec2 vUV;

uniform vec3 ambientMat = vec3(1,1,1);
uniform sampler2D diffuseMap;

out vec4 fragColour;

void main()
{
	vec4 tex = texture(diffuseMap, vUV);

	// Simple transparency
	if(tex.a <= 0) discard;

	fragColour = vec4(tex.xyz, 1) * vec4(ambientMat, 1.0f);
}
