#version 420

layout(location = 0) in vec2 inUV;

out vec4 frag_color;

layout (binding = 0) uniform sampler2D s_screenTex;

uniform float u_PixelSize;

void main() 
{
	//Sample pixels in a horizontal row
	//Weight should add up to 1
	frag_color = vec4(0.0, 0.0, 0.0, 0.0);

	frag_color += texture(s_screenTex, vec2(inUV.x, inUV.y - 4.0 * u_PixelSize)) * 0.06;
	frag_color += texture(s_screenTex, vec2(inUV.x, inUV.y - 3.0 * u_PixelSize)) * 0.09;
	frag_color += texture(s_screenTex, vec2(inUV.x, inUV.y - 2.0 * u_PixelSize)) * 0.12;
	frag_color += texture(s_screenTex, vec2(inUV.x, inUV.y - 	   u_PixelSize)) * 0.15;
	frag_color += texture(s_screenTex, vec2(inUV.x, inUV.y				   )) * 0.16;
	frag_color += texture(s_screenTex, vec2(inUV.x, inUV.y +	   u_PixelSize)) * 0.15;
	frag_color += texture(s_screenTex, vec2(inUV.x, inUV.y + 2.0 * u_PixelSize)) * 0.12;
	frag_color += texture(s_screenTex, vec2(inUV.x, inUV.y + 3.0 * u_PixelSize)) * 0.09;
	frag_color += texture(s_screenTex, vec2(inUV.x, inUV.y + 4.0 * u_PixelSize)) * 0.06;
}