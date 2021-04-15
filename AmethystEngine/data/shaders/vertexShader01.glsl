#version 420

//uniform mat4 MVP;
uniform mat4 matModel;					// Model or World
uniform mat4 matModelInverTrans;		// For normal calculation
uniform mat4 matView; 					// View or camera
uniform mat4 matProj;					// Projection transform

in vec4 vColour;				// Was vec3
in vec4 vPosition;				// Was vec3
in vec4 vNormal;				// Vertex normal
in vec4 vUVx2;					// 2 x Texture coords

out vec4 fColour;
out vec4 fVertWorldLocation;
out vec4 fNormal;
out vec4 fUVx2;

void main() {
    vec4 vertPosition = vPosition;
    mat4 matMVP = matProj * matView * matModel;
	gl_Position = matMVP * vec4(vertPosition.xyz, 1.0);
	
	// Vertex location in "world space"
	fVertWorldLocation = matModel * vec4(vertPosition.xyz, 1.0);
	
 	fNormal = vNormal;
	
	// Pass the colour and UV unchanged.
    fColour = vColour;
	fUVx2 = vUVx2;
}
