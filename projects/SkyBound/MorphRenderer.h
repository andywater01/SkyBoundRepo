#pragma once
#include "Graphics/VertexArrayObject.h"
#include "Gameplay/ShaderMaterial.h"
#include "MorphLoader.h"

class MorphRenderer
{
public:
	VertexArrayObject::sptr vao;

	std::vector<VertexBuffer> vbo;

	void addFrame(std::shared_ptr<MeshBuilder<VertexPosNormTexCol>> mesh)
	{
		vbo.push_back(mesh->returnVBO());
	}


	

};

