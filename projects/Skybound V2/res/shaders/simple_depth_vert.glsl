#version 410

layout (location = 0) in vec3 inPosition;

//Out lightspace matrix
uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_Model;

void main()
{ 
	//Lightspace matrix is the viewProjection matrix from the light's perspective
	gl_Position = u_LightSpaceMatrix * u_Model * vec4(inPosition, 1.0);
}