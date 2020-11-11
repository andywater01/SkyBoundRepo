#version 410
in vec3 color;
in vec2 texUV;
in vec3 world_pos;
in vec3 world_normal;

uniform vec3 light_pos;
uniform vec3 eye_pos;

layout(location = 1) in vec3 inColor;

out vec4 frag_color;

uniform sampler2D myTextureSampler;

const float lightIntensity = 10.0;

//Lecture 10c - Toon shading
const int bands = 4;
const float scaleFactor = 1.0/bands;


void main() { 
	
	vec3 L = normalize(light_pos - world_pos);
	vec3 V = normalize(eye_pos - world_pos);

	float dist = length(light_pos - world_pos);

	float diffuse = max(0, dot(L, world_normal));
	vec3 diffuseOut = (diffuse * color) / (dist*dist);
	
	//Outline effect
	float edge = (dot(V, world_normal) < 0.4) ? 0.0 : 1.0;


	diffuseOut = diffuseOut*lightIntensity;

	//Lecture 10c
	diffuseOut = floor(diffuseOut * bands) * scaleFactor;

	vec3 result = diffuseOut*edge;

	//frag_color = texture(myTextureSampler, texUV)* vec4(result, 1.0);
	
	frag_color = vec4(result, 1.0);
}