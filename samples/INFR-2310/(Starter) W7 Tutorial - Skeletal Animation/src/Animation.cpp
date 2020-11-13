/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Animation.cpp
Classes for managing animation data and blendtrees.
*/

#include "Animation.h"

namespace nou
{
	JointAnim::JointAnim()
	{
		jointInd = 0;
		rotFrames = 0;
		posFrames = 0;
	}

	SkeletalAnim::SkeletalAnim()
	{
		duration = 0.0f;
	}

	SkeletalAnimClip::JointPose::JointPose()
	{
		pos = glm::vec3(0.0f, 0.0f, 0.0f);
		rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}

	SkeletalAnimClip::SkeletalAnimClip(const SkeletalAnim& anim, const Skeleton& skeleton)
		: m_anim(anim)
	{
		m_timer = 0.0f;

		//Should have output for each joint in our skeleton.
		m_result.resize(skeleton.m_joints.size());

		for (size_t i = 0; i < skeleton.m_joints.size(); ++i)
		{
			//Output should default to the base pose of our skeleton.
			m_result[i].pos = skeleton.m_joints[i].m_basePos;
			m_result[i].rotation = skeleton.m_joints[i].m_baseRotation;
		}

		m_rotFrame.resize(m_anim.data.size());
		std::fill(m_rotFrame.begin(), m_rotFrame.end(), 0);

		m_posFrame.resize(m_anim.data.size());
		std::fill(m_posFrame.begin(), m_posFrame.end(), 0);
	}

	void SkeletalAnimClip::Update(float deltaTime, const Skeleton& skeleton)
	{
		//TODO: Implement this function!
		//(You will be REPLACING what is here,
		//not just adding to it.)
		m_timer += deltaTime;


		int p0Index, p1Index;

		for (size_t i = 0; i < m_anim.data.size(); ++i)
		{
			JointPose& pose = m_result[m_anim.data[i].jointInd];

			if (m_anim.data[i].posFrames > 0)
			{
				pose.pos = m_anim.data[i].posKeys[0];
			}

			if (m_anim.data[i].rotFrames == 1)
			{
				pose.rotation = m_anim.data[i].rotKeys[0];
			}
			else if (m_anim.data[i].rotFrames > 0)
			{
				p0Index = m_rotFrame[i];
				p1Index = (p0Index + 1) % m_anim.data[i].rotFrames;

				if (m_timer >= m_anim.data[i].rotTimes[p1Index])
				{
					p0Index = p1Index;
					p1Index = (p0Index + 1) % m_anim.data[i].rotFrames;
					m_rotFrame[i] = (m_rotFrame[i] + 1) % m_anim.data[i].rotFrames;
				}

				float t = fabsf(m_timer - m_anim.data[i].rotTimes[p0Index]) /
						  fabsf(m_anim.data[i].rotTimes[p1Index] - m_anim.data[i].rotTimes[p0Index]);

				pose.rotation = glm::mix(m_anim.data[i].rotKeys[p0Index], m_anim.data[i].rotKeys[p1Index], t);
			}
		}

		if (m_timer >= m_anim.duration)
		{
			m_timer -= m_anim.duration;
		}

	}

	void SkeletalAnimClip::Apply(Skeleton& skeleton)
	{
		//Indices of output match joint indices.
		for (size_t i = 0; i < m_result.size(); ++i)
		{
			Joint& joint = skeleton.m_joints[i];
			joint.m_pos = m_result[i].pos;
			joint.m_rotation = m_result[i].rotation;
		}
	}
}