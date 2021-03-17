#pragma once
#include <GLM/common.hpp>

struct DirectionalLight
{
	//SAME TYPES SHOULD BE GROUPED TOGETHER
	//VEC4s 
	//SHOULD ALWAYS USE VEC4s (Vec3s get upscaled to Vec4s anyways, using anything less is a waste of memory)
	glm::vec4 _lightDirection = glm::vec4(-0.98f, -4.97f, -10.0f, 0.0f); //-0.98f, -4.97f, -10 //Default: 0.f, -3.f, -5.0f, 0.0f
	glm::vec4 _lightCol = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	glm::vec4 _ambientCol = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	
	//FLOATS
	float _ambientPow = 0.6f; //0.6f
	float _lightAmbientPow = 0.05f;
	float _lightSpecularPow = 1.0f;

	float _shadowBias = 0.005;
};