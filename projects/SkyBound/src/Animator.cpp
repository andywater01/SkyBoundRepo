#include "Animator.h"


Animator::AnimData::AnimData()
{
	frameIndex = 0;
	frameTime = 0.2f;
}

Animator::Animator(entt::entity& owner)
{
	m_owner = &owner;
	m_data = std::make_unique<AnimData>();
	m_timer = 0.0f;
}



void Animator::Update(float deltaTime)
{
	if (!&m_data->frames)
		return;

	float t;

	if (m_data->frameTime > 0.0f)
	{
		m_timer += deltaTime;

		if (m_timer > m_data->frameTime)
		{
			m_timer -= m_data->frameTime;

			m_data->frameIndex += 1;

			if (m_data->frameIndex >= m_data->frames.size())
				m_data->frameIndex = 0;

		}

		m_timer = fmod(m_timer, m_data->frameTime);

		t = m_timer / m_data->frameTime;

	}
	else
	{
		t = 0.0f;
	}

	size_t f0Index;
	size_t f1Index;

	f1Index = m_data->frameIndex;

	if (f1Index == 0)
	{
		f0Index = m_data->frames.size() - 1;
	}
	else
	{
		f0Index = f1Index - 1;
	}


}



void Animator::SetFrames(const std::vector<std::unique_ptr<MeshBuilder<VertexPosNormTexColAnim1>>>& objectFrames)
{
	m_data->frames.clear();
	m_data->frames.reserve(objectFrames.size());

	for (int x = 0; x < objectFrames.size(); x++)
	{
		m_data->frames.push_back(objectFrames[x].get());
	}
}




void Animator::SetFrameTime(float frameTime)
{
	m_data->frameTime = frameTime;
}
