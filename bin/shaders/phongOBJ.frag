#version 410

in vec4 vPosition;	// Vertex World Position
in vec2 vUV;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;

// Material
uniform sampler2D diffuseMap;	// material diffuse map
uniform sampler2D specularMap;	// material specular map
uniform sampler2D normalMap;	// material normal map

// Lighting
uniform float specularLightPower = 16.0f;	// specular power

uniform vec3 lightDir = vec3(0.5f, 0.5f, 0);	// normalised direction from light to camera
uniform vec3 cameraPos = vec3(0, 0, 0);	// world-space camera position
uniform vec3 ambientLight = vec3(0.3f, 0.3f, 0.3f); // environment ambient light

out vec4 fragColour;

void main()
{
	// Calculate Surface Normal
	mat3 TBN = mat3( 	normalize( vTangent ),		// UV Normal to Object-space matrix
			 			normalize( vBiTangent ),
						normalize( vNormal ) );
	vec3 uvNormal = texture( normalMap, vUV ).xyz * 2 - 1;	// Normal from Map
	vec3 wsNormal = normalize(TBN * uvNormal);	// Normal in object-space
	// ------------------------

	// PHONG LIGHTING ----------------------------
	
	float d = max( 0.0f, dot( wsNormal, lightDir ) );	// diffuse light

	vec3 E = normalize( cameraPos - vPosition.xyz );	// surface-to-eye vector
	vec3 R = reflect( -lightDir, wsNormal );		//reflected light vector
	
	float specTerm =  clamp( dot( R, E ), 0.f, 1.f );	// Specular Term
	specTerm = pow( specTerm, specularLightPower);

	vec3 texColour = texture( diffuseMap, vUV ).xyz;	// Diffuse Colour
	vec3 Ambient = ambientLight * texColour;// * ambientMat;	// ambient light
	
	vec3 Diffuse = d * texColour;	// diffuse light for 1 light
	vec3 Specular = texture( specularMap, vUV ).xyz * specTerm; // specular light for 1 light

	fragColour = vec4( Ambient + Diffuse + Specular, 1.0f); // Ambient + Diffuse + Specular
	// -------------------------------------------
}
