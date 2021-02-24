#version 420

layout(location = 0) in vec2 inUV;

out vec4 frag_color;

layout (binding = 0) uniform sampler2D s_screenTex;
layout(binding = 1) uniform sampler2D u_Bloom;

void main() 
{
	vec4 colorA = texture(s_screenTex, inUV);
	vec4 colorB = texture(u_Bloom, inUV);

	frag_color = 1.0 - (1.0 - colorA) * (1.0 - colorB);
}