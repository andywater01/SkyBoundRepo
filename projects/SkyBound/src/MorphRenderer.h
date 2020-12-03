#pragma once
#include "Graphics/VertexArrayObject.h"
#include "Gameplay/ShaderMaterial.h"
#include "MorphLoader.h"
#include "Gameplay/Transform.h"

class MorphRenderer
{
public:
	VertexArrayObject::sptr vao;

	//std::vector<VertexBuffer::sptr> vbo;
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
		Transform& transform);


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


};

