#pragma once

#include "FSM/FSM.h"
#include "NOU/Entity.h"

namespace nou
{
	class CSkullFSM : public FSM
	{
	public:

		static const std::string idleClip;
		static const std::string attackClip;
		static const std::string emoteClip;

		enum class AnimState
		{
			IDLE = 0,
			ATTACK,
			EMOTE
		};

		CSkullFSM(Entity& owner);
		~CSkullFSM() = default;

		CSkullFSM(CSkullFSM&&) = default;
		CSkullFSM& operator=(CSkullFSM&&) = default;

		void SetState(AnimState state);
		void Update();

	private:

		Entity* m_owner;
		AnimState m_state;
	};
}