#version 410

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;

uniform sampler2D s_Diffuse;
uniform sampler2D s_Specular;
uniform sampler2D s_Diffuse2;
uniform sampler2D s_Diffuse3;

uniform vec3  u_AmbientCol;
uniform float u_AmbientStrength;

uniform vec3  u_LightPos;
uniform vec3  u_LightCol;
uniform float u_AmbientLightStrength;
uniform float u_SpecularLightStrength;
uniform float u_Shininess;
// NEW in week 7, see https://learnopengl.com/Lighting/Light-casters for a good reference on how this all works, or
// https://developer.valvesoftware.com/wiki/Constant-Linear-Quadratic_Falloff
uniform float u_LightAttenuationConstant;
uniform float u_LightAttenuationLinear;
uniform float u_LightAttenuationQuadratic;

uniform float u_TextureMix;
uniform float u_OutlineThickness;

uniform vec3  u_CamPos;

out vec4 frag_color;
const float lightIntensity = 10.0;

//Toon Shading
const int bands = 3;
const float scaleFactor = 1.0/bands;

// https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
void main() {
	// Lecture 5
	vec3 ambient = u_AmbientLightStrength * u_LightCol;

	// Diffuse
	vec3 N = normalize(inNormal);
	vec3 lightDir = normalize(u_LightPos - inPos);

	float dist = length(u_LightPos - inPos);

	//float dif = max(dot(N, lightDir), 0.0);
	float dif = max(0.0, dot(lightDir, N));
	vec3 diffuse = dif * u_LightCol;// add diffuse intensity

	//vec3 diffuseOut = (dif * u_LightCol) / (dist*dist);
	//diffuseOut = diffuseOut * lightIntensity;

	//diffuse = floor(diffuseOut * bands) * scaleFactor;

	//Attenuation
	float attenuation = 1.0f / (
		u_LightAttenuationConstant + 
		u_LightAttenuationLinear * dist +
		u_LightAttenuationQuadratic * dist * dist);

	// Specular
	vec3 viewDir  = normalize(u_CamPos - inPos);
	vec3 h        = normalize(lightDir + viewDir);

	// Get the specular power from the specular map
	float texSpec = texture(s_Specular, inUV).x;
	float spec = pow(max(dot(N, h), 0.0), u_Shininess); // Shininess coefficient (can be a uniform)
	vec3 specular = u_SpecularLightStrength * texSpec * spec * u_LightCol; // Can also use a specular color

	// Get the albedo from the diffuse / albedo map
	vec4 textureColor = texture(s_Diffuse, inUV);
	vec4 textureColor2 = texture(s_Diffuse2, inUV);

	vec4 mixedTextureColor = mix(textureColor, textureColor2, u_TextureMix);

	//Outline Effect             Thickness of Line
	float edge = (dot(viewDir, N) < u_OutlineThickness) ? 0.0 : 1.0; //If below threshold it is 0, otherwise 1

	vec3 lightContribution = (ambient + diffuse + specular) * attenuation;

	lightContribution = clamp(floor(lightContribution * bands) * scaleFactor, vec3(0.0), vec3(1.0));

	vec3 result = (u_AmbientCol * u_AmbientStrength +
		lightContribution) *
						// global ambient light
						// light factors from our single light
		  inColor * mixedTextureColor.rgb * edge; // Object color

	
	vec3 debug = (viewDir - vec3(0.5)) * 2;

	frag_color = vec4(result, mixedTextureColor.a);
}