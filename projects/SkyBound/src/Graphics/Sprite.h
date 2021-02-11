#pragma once
#include "Graphics/VertexArrayObject.h"
#include "Gameplay/ShaderMaterial.h"

class Sprite
{
public:
	VertexArrayObject::sptr Mesh;
	ShaderMaterial::sptr    Material;

	Sprite& SetMesh(const VertexArrayObject::sptr& mesh) { Mesh = mesh; return *this; }
	Sprite& SetMaterial(const ShaderMaterial::sptr& material) { Material = material; return *this; }
};

