#pragma once

#include "MeshBuilder.h"
#include "MeshFactory.h"
//#include "VertexTypes.h"

class ObjLoader
{
public:
	static bool LoadFromFile(const std::string& filename, std::vector<glm::vec3>& outVec, std::vector<glm::vec2>& outUV, std::vector<glm::vec3>& outNorm);
	
protected:
	ObjLoader() = default;
	~ObjLoader() = default;
};

