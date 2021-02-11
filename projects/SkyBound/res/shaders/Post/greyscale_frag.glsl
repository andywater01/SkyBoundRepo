#version 420

layout(location = 0) in vec2 inUV;

out vec4 frag_color;

layout (binding = 0) uniform sampler2D s_screenTex;

//Affects how greyscale
//Lower the number, closer to regular color
uniform float u_Intensity = 1.0;

void main() 
{
	vec4 source = texture(s_screenTex, inUV);

	float luminence = 0.2989 * source.r + 0.587 * source.g + 0.114 * source.b;

	frag_color.rgb = mix(source.rgb, vec3(luminence), u_Intensity);
    frag_color.a = source.a;
}