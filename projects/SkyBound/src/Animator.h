#pragma once
#include <string>
#include "Utilities/MeshBuilder.h"
#include "Utilities/MeshFactory.h"
#include "Gameplay/Scene.h"

class Animator
{
public:
	Animator(entt::entity& owner);
	~Animator() = default;

	Animator& operator=(Animator&&) = default;

	void Update(float deltaTime);

	void SetFrames(const std::vector<std::unique_ptr<MeshBuilder<VertexPosNormTexColAnim1>>> &objectFrames);
	void SetFrameTime(float frameTime);

protected:

	entt::entity* m_owner;

	class AnimData {
	public:
		std::string filename;

		std::vector<const MeshBuilder<VertexPosNormTexColAnim1>*> frames;

		float frameTime;

		int frameIndex;

		AnimData();

		~AnimData() = default;

	};

	std::unique_ptr<AnimData> m_data;

	float m_timer;

};

