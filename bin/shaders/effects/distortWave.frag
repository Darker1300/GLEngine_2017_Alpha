#version 410

in vec2 vUV;
in vec3 vPosition;

uniform sampler2D colourMap;
uniform sampler2D depthMap;

uniform float distortTime;

out vec4 fragColour;

float CalcDepth() {
	float n = 0.1f; 		//near
	float f = 100.0f; 		// far
	float z = vPosition.z;	// z distance
	return (((2.0f * z - n - f) / (f - n)) * 0.5f) + 0.5f; 
}

void main()
{
	//float distort = vUV.y;// ( cos(distortTime / 1000) + vUV.y + 1 ) * 0.5f;
	// vec2 distort = ( vec2( sin( vUV.x + distortTime), cos( vUV.y + distortTime ) ) + 1 ) * 0.5;
	vec2 texcoord = vUV;
  	texcoord.x += sin(texcoord.y * 4*2*3.14159 + distortTime) / 100;
	
	vec4 colourTex = texture(colourMap, texcoord);
	vec4 depthTex = texture(depthMap, vUV);
	float d = CalcDepth();
	fragColour = colourTex ;//* vec4(d, d, d, depthTex.a);

	//fragColour = colourTex;
}