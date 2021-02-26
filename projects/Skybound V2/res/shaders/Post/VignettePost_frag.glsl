#version 420

out vec4 FragColor;

//in vec2 TexCoords;
layout(location = 0) in vec2 inUV;

//layout(binding = 0) uniform sampler2D uTex;
layout(binding = 0) uniform sampler2D s_screenTex;

uniform float uInnerRadius = 0.2;
uniform float uOuterRadius = 1.0;
uniform float uOpacity = 1.0;

void main() 
{
    //Load in the source image
	vec4 source = texture(s_screenTex, inUV);
    //Create the vignette (is white right now)
    vec4 vignette = vec4(1.0);

    //Get vector to center
	vec2 toCenter = inUV - vec2(0.5);
    
    //Create vignette effect
    vignette.rgb *= 1.0 - smoothstep(uInnerRadius, uOuterRadius, length(toCenter));

    //Apply the vignette to the image
    vignette *= source;

    //Keep the opacity uniform relevant by mixing between the original and vignetted image based on opacity
    vignette = mix(source, vignette, uOpacity);

	FragColor.rgb = vignette.rgb;
	FragColor.a = 1.0f;
}