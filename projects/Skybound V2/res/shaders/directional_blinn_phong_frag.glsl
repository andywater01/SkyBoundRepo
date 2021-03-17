#version 420

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;
layout(location = 4) in vec4 inFragPosLightSpace;

struct DirectionalLight
{
	//Light direction (defaults to down, to the left, and a little forward)
	vec4 _lightDirection;

	//Generic Light controls
	vec4 _lightCol;

	//Ambience controls
	vec4 _ambientCol;
	float _ambientPow;
	
	//Power controls
	float _lightAmbientPow;
	float _lightSpecularPow;

	float _shadowBias;
};

layout (std140, binding = 0) uniform u_Lights
{
	DirectionalLight sun;
};

layout (binding = 30) uniform sampler2D s_ShadowMap;
uniform sampler2D s_Diffuse;
uniform sampler2D s_Diffuse2;
uniform sampler2D s_Specular;

uniform float u_TextureMix;
uniform vec3  u_CamPos;

out vec4 frag_color;

//Toon Shading
uniform float u_OutlineThickness;
const int bands = 3;
const float scaleFactor = 1.0/bands;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	//Perspective division
	vec3 projectionCoordinates = fragPosLightSpace.xyz / fragPosLightSpace.w;

	//Transform into a [0, 1] range
	projectionCoordinates = projectionCoordinates * 0.5 + 0.5;

	//Get the closest depth value
	float closestDepth = texture(s_ShadowMap, projectionCoordinates.xy).r;

	//Get the current depth according to our camera
	float currentDepth = projectionCoordinates.z;

	//Hard Shadows
	//Check whether there's a shadow
	//float shadow = currentDepth - sun._shadowBias > closestDepth ? 1.0 : 0.0;


	//PCF
	float shadow = 0.0;

	vec2 texelSize = 1.0 / textureSize(s_ShadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(s_ShadowMap, projectionCoordinates.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - sun._shadowBias > pcfDepth ? 1.0 : 0.0;
		}
	}

	shadow /= 9.0;

	return shadow;
}

// https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
void main() {
	// Diffuse
	vec3 N = normalize(inNormal);
	vec3 lightDir = normalize(-sun._lightDirection.xyz);
	float dif = max(dot(N, lightDir), 0.0);
	vec3 diffuse = dif * sun._lightCol.xyz;// add diffuse intensity

	// Specular
	vec3 viewDir  = normalize(u_CamPos - inPos);
	vec3 h        = normalize(lightDir + viewDir);

	// Get the specular power from the specular map
	float texSpec = texture(s_Specular, inUV).x;
	float spec = pow(max(dot(N, h), 0.0), 4.0); // Shininess coefficient (can be a uniform)
	vec3 specular = sun._lightSpecularPow * texSpec * spec * sun._lightCol.xyz; // Can also use a specular color

	vec3 ambient = sun._ambientCol.xyz * sun._ambientPow;

	// Get the albedo from the diffuse / albedo map
	vec4 textureColor1 = texture(s_Diffuse, inUV);
	vec4 textureColor2 = texture(s_Diffuse2, inUV);
	vec4 textureColor = mix(textureColor1, textureColor2, u_TextureMix);

	float shadow = ShadowCalculation(inFragPosLightSpace);

	//Outline Effect             Thickness of Line
	float edge = (dot(viewDir, N) < u_OutlineThickness) ? 0.0 : 1.0; //If below threshold it is 0, otherwise 1

	vec3 lightContribution = (diffuse + specular);

	lightContribution = clamp(floor(lightContribution * bands) * scaleFactor, vec3(0.0), vec3(1.0));

	vec3 result = (
		(sun._ambientPow * sun._ambientCol.xyz) + // global ambient light
		(1.0 - shadow) * (lightContribution) // light factors from our single light
		) * inColor * textureColor.rgb * edge; // Object color

//	vec3 result = (
//		(sun._ambientPow * sun._ambientCol.xyz) + // global ambient light
//		(1.0 - shadow) * (diffuse + specular) // light factors from our single light
//		) * inColor * textureColor.rgb; // Object color

	frag_color = vec4(result, textureColor.a);
}