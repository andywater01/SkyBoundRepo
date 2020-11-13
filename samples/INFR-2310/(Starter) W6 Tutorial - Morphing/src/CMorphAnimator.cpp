		/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CMorphAnimator.cpp
Simple animator component for demonstrating morph target animation.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#include "CMorphAnimator.h"
#include "CMorphMeshRenderer.h"
#include "NOU/Mesh.h"

namespace nou
{
	
	CMorphAnimator::AnimData::AnimData()
	{
		frame0 = nullptr;
		frame1 = nullptr;
		frameIndex = 0;
		frameTime = 0.2f;
	}
	


	CMorphAnimator::CMorphAnimator(Entity& owner)
	{
		m_owner = &owner;
		m_data = std::make_unique<AnimData>();
		m_timer = 0.0f;
		m_forwards = true;
	}


	void CMorphAnimator::Update(float deltaTime)
	{
		//TODO: Complete this function.
		if (!&m_data->boiFrames)
			return;

		float t;

		if (m_data->frameTime > 0.0f)
		{
			m_timer += deltaTime;

			if (m_timer > m_data->frameTime)
			{
				m_timer -= m_data->frameTime;

				m_data->frameIndex += 1;

				if (m_data->frameIndex >= m_data->boiFrames.size())
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
			f0Index = m_data->boiFrames.size() - 1;
		}
		else
		{
			f0Index = f1Index - 1;
		}


		m_owner->Get<CMorphMeshRenderer>().UpdateData(*m_data->boiFrames[f0Index], *m_data->boiFrames[f1Index], t);

	}

	void CMorphAnimator::SetFrames(const std::vector<std::unique_ptr<Mesh>> &boiFrames)
	{
		m_data->boiFrames.clear();
		m_data->boiFrames.reserve(boiFrames.size());

		for (int x = 0; x < boiFrames.size(); x++)
		{
			m_data->boiFrames.push_back(boiFrames[x].get());
		}
	}

	void CMorphAnimator::SetFrameTime(float frameTime)
	{
		m_data->frameTime = frameTime;
	}
}