#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include "glm/common.hpp"

class LUT3D
{
public:
	LUT3D();
	LUT3D(std::string path);
	void loadFromFile(std::string path);
	void bind();
	void unbind();

	void bind(int textureSlot);
	void unbind(int textureSlot);
private:
	GLuint _handle = GL_NONE;
	std::vector<glm::vec3> data;
};