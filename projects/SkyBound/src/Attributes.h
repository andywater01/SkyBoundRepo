#pragma once
#include <GLM/glm.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

struct Attributes
{
public:
	glm::vec3 curPos;
	glm::vec3 endPos;
	glm::vec3 startPos;
	glm::quat curRot;
	glm::quat endRot;

};