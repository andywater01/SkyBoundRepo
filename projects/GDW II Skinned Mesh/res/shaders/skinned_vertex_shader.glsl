#version 410
#define MAX_JOINTS 20

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;

// Skinned mesh input
layout(location = 4) in vec4 inJoint;
layout(location = 5) in vec4 inWeight;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outColor;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec2 outUV;

uniform mat4 u_ModelViewProjection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat3 u_NormalMatrix;
uniform vec3 u_LightPos;

// Skinned mesh uniforms
uniform mat4[MAX_JOINTS] u_JointMatrices;

void main() {

	vec3 pos = inPosition;
	
	vec4 weight = inWeight;

	// Skeletal Animation
	mat4 skinMatrix = weight.x * u_JointMatrices[int(inJoint.x)] +
					  weight.y * u_JointMatrices[int(inJoint.y)] + 
					  weight.z * u_JointMatrices[int(inJoint.z)] +
					  weight.w * u_JointMatrices[int(inJoint.w)];

	gl_Position = u_ModelViewProjection * skinMatrix * vec4(pos, 1.0);

	// Standard vertex shader business
	// Pass vertex pos in world space to frag shader
	outPos = (gl_Position).xyz + (0, 1, 0);

	// Normals
	outNormal = u_NormalMatrix * inNormal;

	// Pass our UV coords to the fragment shader
	outUV = inUV;

	///////////
	outColor = inColor;

}

