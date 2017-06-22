#version 410

in vec4 vPosition;
in vec2 vUV;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;

// Material
uniform sampler2D normal; // material normal map
uniform vec3 kA = vec3(0,0,0); // ambient material color.
uniform sampler2D kD; // material diffuse map
uniform sampler2D kS; // material specular map

// Lighting
uniform vec3 iA = vec3(0.25f, 0.25f, 0.25f); // environment ambient light
uniform vec3 iD = vec3(1,1,1);	 // environment diffuse light
uniform vec3 iS = vec3(1,1,1);	 // environment specular light
uniform float iSpecPower = 32.0f; // specular power

uniform vec3 cameraPos; // world-space camera position
//uniform vec3 L; // normalised light direction from light
uniform vec3 lightDir;

out vec4 fragColour;

void main()
{
	vec3 fNormal = texture( normal, vUV ).xyz * 2 - 1; // Normal from Map

	mat3 TBN = mat3( 	normalize( vTangent ),		// 
			 			normalize( vBiTangent ),
						normalize( fNormal ) );

	float d = max( 0, dot(normalize( TBN * fNormal ), normalize(lightDir)));


	// vec3 AmbientL = kA * iA;	// ambient light
	
	// float Ndl = max( 0.0f, dot( N, -L ) );	// Lambert term
	// vec3 DiffuseL = vec3( texture( kD, vUV ) ) * iD * Ndl;	// diffuse ligt for one light
	
	vec3 R = reflect( -lightDir, fNormal );	//reflected light vector
	vec3 E = normalize( cameraPos - vPosition.xyz );	// surface-to-eye vector
	float s = max( 0, dot( E, R ) );
	s = pow( s, iSpecPower );
	// float specTerm = pow( min ( 0.0f, dot( R, E ) ), iSpecPower );	// Specular term
	// vec3 SpecularL = vec3( texture( kS, vUV ) ) * iS * specTerm;	// specular light for one light

	// fragColour = vec4(AmbientL + DiffuseL + SpecularL, 1);
	fragColour = vec4(iA * d + iA * s * fragColour.rgb, 1);

	//fragColour = texture(kD, vUV);
	//fragColour.rgb = fragColour.rgb * d;
}