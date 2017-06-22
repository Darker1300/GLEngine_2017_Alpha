#version 410

in vec4 vPosition;	// Vertex World Position
in vec2 vUV;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;

// Material
uniform vec3 ambientMat = vec3(0,0,0);	// ambient material color.
uniform sampler2D diffuseMap;	// material diffuse map
uniform sampler2D specularMap;	// material specular map
uniform sampler2D normalMap;	// material normal map

// Lighting
uniform vec3 ambientLight = vec3(0.5f, 0.5f, 0.5f); // environment ambient light
uniform vec3 diffuseLight = vec3(0.5f, 0.5f, 0.5f);	// environment diffuse light
uniform vec3 specularLight = vec3(0.9f, 0.9f, 0.9f);	// environment specular light
uniform float specularLightPower = 16.0f;	// specular power

uniform vec3 cameraPos;	// world-space camera position
uniform vec3 lightDir;	// normalised light direction from light
uniform float roughness;	// surface roughness. UNUSED WITH PHONG

out vec4 fragColour;

void main()
{
	// Calculate Surface Normal
	vec3 uvNormal = texture( normalMap, vUV ).xyz * 2 - 1;	// Normal from Map
	mat3 TBN = mat3( 	normalize( vTangent ),		// UV to Object-space matrix
			 			normalize( vBiTangent ),
						normalize( vNormal ) );
	vec3 fNormal = TBN * uvNormal;	// Normal in object-space
	// ------------------------

	// PHONG LIGHTING ----------------------------
	vec3 Ambient = ambientMat * ambientLight;	// ambient light
	
	float NdL = max( 0.0f, dot( fNormal, -lightDir ) ); 			// Lambert Term
	vec3 Diffuse = texture( diffuseMap, vUV ).xyz * diffuseLight;	// diffuse light for 1 light

	vec3 R = reflect( lightDir, fNormal );		//reflected light vector
	vec3 E = normalize( cameraPos - vPosition.xyz );	// surface-to-eye vector

	float specTerm = pow( clamp( dot( R, E ), 0.f, 1.f ), specularLightPower );	// Specular Term
	vec3 Specular = texture( specularMap, vUV ).xyz * specularLight * specTerm; // specular light for 1 light

	fragColour = vec4( Ambient + Diffuse + Specular, 1.0f); // Ambient + Diffuse + Specular
	// -------------------------------------------
}