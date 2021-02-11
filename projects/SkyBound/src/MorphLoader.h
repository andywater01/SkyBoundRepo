#pragma once
#include "Utilities/MeshFactory.h"
//#include "Gameplay/Scene.h"
#include "Graphics/Shader.h"


class MorphLoader
{
public:
	static std::shared_ptr<MeshBuilder<VertexPosNormTexCol>> LoadFromFile(const std::string& filename, const glm::vec4& inColor = glm::vec4(1.0f));

protected:
	MorphLoader() = default;
	~MorphLoader() = default;

};

