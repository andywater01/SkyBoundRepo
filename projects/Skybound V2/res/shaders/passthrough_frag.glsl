#version 420

layout(location = 0) in vec2 inUV;

out vec4 frag_color;

layout (binding = 0) uniform sampler2D s_screenTex;
uniform float u_Transparency = 1.0f;

void main() 
{
	vec4 source = texture(s_screenTex, inUV);

	frag_color.rgb = source.rgb;
	frag_color.a = source.a * u_Transparency;
}