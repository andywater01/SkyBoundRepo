#version 410

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;

uniform vec3 LightPos;

out vec4 frag_color;


void main() {
	
	//Lecture 5
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	//Ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor * inColor;

	//Diffuse
	vec3 N = normalize(inNormal);
	vec3 lightDir = normalize(LightPos - inPos);

	float dif = max(dot(N, lightDir), 0.0);
	vec3 diffuse = dif * inColor;

	//Attenuation
	float dist = length(LightPos - inPos);
	diffuse = diffuse / dist;

	//Specular
    vec3 camPos = vec3(0.0, 0.0, 3.0);
    float specularStrength = 2.0;
    vec3 camDir = normalize(camPos - N);
    //vec3 reflectDir = reflect(-lightDir, N);
    vec3 halfway = (camDir + (-lightDir));
    //float spec = pow(max(dot(camDir, reflectDir), 0.0), 128); //Shininess coefficient
    float blinnAngle = dot(halfway, -inPos);
    //vec3 specular = specularStrength * spec * lightColor;
    vec3 specular = specularStrength * blinnAngle * lightColor;

    vec3 result = (ambient + diffuse + specular);

    frag_color = vec4(result, 1.0);
	
}