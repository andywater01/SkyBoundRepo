#version 410

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;

layout(location = 4) in vec3 inPosition2;
layout(location = 5) in vec3 inNormal2;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outColor;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec2 outUV;
layout(location = 4) out vec4 outFragPosLightSpace;


uniform mat4 u_ModelViewProjection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat3 u_ModelRotation;
uniform vec3 u_LightPos;
uniform mat3 u_NormalMatrix;
uniform mat4 u_LightSpaceMatrix;
uniform float t;

void main() {

	
	// Pass vertex pos in world space to frag shader
	vec3 modelPos = mix(inPosition, inPosition2, t);

	outPos = (u_Model * vec4(modelPos, 1.0)).xyz;

	//outPos = mix(vec4(inPosition, 1.0), vec4(inPosition2, 1.0), t)).xyz;


	// Normals
	outNormal = u_NormalMatrix * mix(inNormal, inNormal2, t);

	// Pass our UV coords to the fragment shader
	outUV = inUV;

	//Pass out the light space fragment pos
	outFragPosLightSpace = u_LightSpaceMatrix * vec4(outPos, 1.0);

	///////////
	outColor = inColor;

	gl_Position = u_ModelViewProjection * vec4(modelPos, 1.0);

}



