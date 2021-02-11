#pragma once
#include "VertexArrayObject.h"
#include "ShaderMaterial.h"
#include "MorphLoader.h"
#include <Transform.h>

class MorphRenderer
{
public:
	VertexArrayObject::sptr vao = VertexArrayObject::Create();

	ShaderMaterial::sptr Material;

	MorphRenderer();
	

	MorphRenderer& operator=(MorphRenderer&&) = default;

	MorphRenderer(MorphRenderer&&) = default;




	void addFrame(std::shared_ptr<MeshBuilder<VertexPosNormTexCol>> mesh);


	void nextFrame(float dt);


	void UpdateData(VertexBuffer::sptr frame0, VertexBuffer::sptr frame1, float t);
	

	void SetFrameTime(float frameTime);
	

	void render(
		const Shader::sptr& shader,
		const glm::mat4& viewProjection,
		Transform& transform, const glm::mat4& view, const glm::mat4& projection);


	MorphRenderer& SetMesh(const VertexArrayObject::sptr& mesh) { vao = mesh; return *this; }
	MorphRenderer& SetMaterial(const ShaderMaterial::sptr& material) { Material = material; return *this; }


	protected:


	class AnimData
	{
	public:

		std::vector<VertexBuffer::sptr> frame;

		//The time inbetween frames.
		float frameTime;
		int frameIndex;

		AnimData();
		~AnimData() = default;
	};


	std::unique_ptr<AnimData> m_anim;

	float m_timer;

	float m_t;

private:
	VertexPosNormTexCol* VPNTC = nullptr;


};

