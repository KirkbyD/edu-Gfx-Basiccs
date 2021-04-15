#version 420

in vec4 fColour;
in vec4 fVertWorldLocation;
in vec4 fNormal;
in vec4 fUVx2;

uniform vec4 diffuseColour;
uniform vec4 specularColour;

// Used to draw debug (or unlit) objects
uniform vec4 debugColour;
uniform bool bDoNotLight;

uniform vec4 eyeLocation;

out vec4 pixelColour;			// RGB A   (0 to 1)

// Fragment shader
struct sLight {
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	vec4 atten;
	vec4 direction;
	vec4 param1;
	vec4 param2;

	/* LIGHT NOTES
		SPECULAR:
			RGB = Highlight Colour
			W = Power
		ATTENUATION (ATTN):
			X = Constant Attenuation
			Y = Linear Attenuation
			Z = Quadratic Attenuation
			W = Distance Cut Off
		DIRECTION:
			Spot
			Directional Lights
		PARAM 1:
			X = Light Type ( Point, Spot, or Directional )
				0 = Point Light ( lamp, overhead light, etc. )
				1 = Spot Light ( flashlight, searchlight, etc. )
				2 = Directional Light ( sunlight / moonlight etc. )
			Y = Inner Angle
			Z = Outer Angle
			W = TBD
		PARAM 2:
			// Light State ( on / off )
				X = 0 ( OFF )
				X = 1 (  ON )
	*/
};

// Light Types
const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;

// Number of lights in the light array.
// for binding of objects perhaps have different light arrays for objects like if its a BOUND_LIGHT_OBJECT (_bound_light_object )
const int NUMBEROFLIGHTS = 10;
uniform sLight theLights[NUMBEROFLIGHTS];  	// 80 uniforms
// Really appears as:
// uniform vec4 theLights[0].position
// uniform vec4 theLights[0].diffuse
// uniform vec4 theLights[0].specular
// uniform vec4 theLights[0].atten
// uniform vec4 theLights[0].direction
// uniform vec4 theLights[0].param1
// uniform vec4 theLights[0].param2

vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, vec3 vertexWorldPos, vec4 vertexSpecular );
	 
void main() {
	if ( bDoNotLight ) {
		pixelColour.rgb = debugColour.rgb;
		pixelColour.a = 1.0f;				// NOT transparent
		return;
	}
	
	vec4 materialColour = diffuseColour;
	vec4 outColour = calcualteLightContrib( materialColour.rgb, fNormal.xyz, fVertWorldLocation.xyz, specularColour );

	
	pixelColour = outColour;
}



vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, vec3 vertexWorldPos, vec4 vertexSpecular ) {
	vec3 norm = normalize(vertexNormal);
	
	vec4 finalObjectColour = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	
	for ( int index = 0; index < NUMBEROFLIGHTS; ++index ) {	
		// LIGHT ENABLED CHECK
		if ( theLights[index].param2.x == 0.0f )
			continue;
		
		// Cast to an int (note with c'tor)
		int intLightType = int(theLights[index].param1.x);
		
		// We will do the directional light here (BEFORE the attenuation, since sunlight has no attenuation, really)
		// DIRECTIONAL ( X = 2 )
		if ( intLightType == DIRECTIONAL_LIGHT_TYPE ) {
			// Simulates Sunlight/Moonlight so there should never be more then one per scene ( cheapest light to calculate )
			vec3 lightContrib = theLights[index].diffuse.rgb;
			
			// Get the dot product of the light and normalize
			float dotProduct = dot( -theLights[index].direction.xyz, normalize(norm.xyz) );	// -1 to 1

			dotProduct = max( 0.0f, dotProduct );		// 0 to 1

			lightContrib *= dotProduct;
			
			// NO ATTENUATION
			finalObjectColour.rgb += (vertexMaterialColour.rgb * theLights[index].diffuse.rgb * lightContrib); 

			return finalObjectColour;
		}
		
		// Default Lighttype = Point Light or X = 0
		
		// Contribution for this light
		vec3 vLightToVertex = theLights[index].position.xyz - vertexWorldPos.xyz;
		float distanceToLight = length(vLightToVertex);	
		vec3 lightVector = normalize(vLightToVertex);

		// -1 to 1
		float dotProduct = dot(lightVector, vertexNormal.xyz);	 
		
		// If it's negative, will clamp to 0 --- range from 0 to 1
		dotProduct = max( 0.0f, dotProduct );	
		
		vec3 lightDiffuseContrib = dotProduct * theLights[index].diffuse.rgb;
			

		// Specular 
		vec3 lightSpecularContrib = vec3(0.0f);
			
		vec3 reflectVector = reflect( -lightVector, normalize(norm.xyz) );

		// Get eye or view vector: the location of the vertex in the world to your eye
		vec3 eyeVector = normalize(eyeLocation.xyz - vertexWorldPos.xyz);

		// To simplify, we are NOT using the light specular value, just the object’s.
		float objectSpecularPower = vertexSpecular.w; 
		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower ) * theLights[index].specular.rgb;

		// Attenuation
		float attenuation = 1.0f / 
				( theLights[index].atten.x + 										
				  theLights[index].atten.y * distanceToLight +						
				  theLights[index].atten.z * distanceToLight*distanceToLight );  	

		// total light contribution is Diffuse + Specular
		lightDiffuseContrib *= attenuation;
		lightSpecularContrib *= attenuation;
		
		
		// SPOT LIGHT ( X = 1 )
		if ( intLightType == SPOT_LIGHT_TYPE ) {
			// Calcualate light vector (light to vertex, in world)
			vec3 vertexToLight = vertexWorldPos.xyz - theLights[index].position.xyz;

			vertexToLight = normalize(vertexToLight);

			float currentLightRayAngle = dot( vertexToLight.xyz, theLights[index].direction.xyz );
			currentLightRayAngle = max(0.0f, currentLightRayAngle);

			// Is this inside the cone? 
			float outerConeAngleCos = cos(radians(theLights[index].param1.z));
			float innerConeAngleCos = cos(radians(theLights[index].param1.y));

			// Is the object in the spot light beam?
			if ( currentLightRayAngle < outerConeAngleCos ) {
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}

			else if ( currentLightRayAngle < innerConeAngleCos ) {
				// Angle is between the inner and outer cone (this is called the penumbra of the spot light, by the way)
				// This blends the brightness from full brightness, near the inner cone to black, near the outter cone
				float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / (innerConeAngleCos - outerConeAngleCos);

				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
			}
		}					
		finalObjectColour.rgb += (vertexMaterialColour.rgb * lightDiffuseContrib.rgb) + (vertexSpecular.rgb  * lightSpecularContrib.rgb );
	}
	finalObjectColour.a = 1.0f;
	return finalObjectColour;
}
