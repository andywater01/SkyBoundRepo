#version 410
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColour;

layout(location = 1) out vec3 outColour;

void main()
{
	// vertex position in clip space
	gl_Position = vec4(inPosition, 1.0);

	outColour = inColour;
}