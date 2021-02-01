#include "CSKullFSM.h"
#include "Sprites/CSpriteAnimator.h"

namespace nou
{
	//String labels for animations.
	const std::string CSkullFSM::idleClip = "skullIdle";
	const std::string CSkullFSM::attackClip = "skullAttack";
	const std::string CSkullFSM::emoteClip = "skullEmote";

	CSkullFSM::CSkullFSM(Entity& owner)
	{
		m_owner = &owner;
		m_state = AnimState::IDLE;

		SetState(m_state);
	}

	void CSkullFSM::SetState(CSkullFSM::AnimState state)
	{
		m_state = state;

		//Gets the animator from the entity.
		auto& animator = m_owner->Get<CSpriteAnimator>();

		ClearTriggers();

		//TODO: Complete this function.
		switch (m_state)
		{
		case AnimState::IDLE:
			animator.PlayLoop(idleClip);
			break;
		case AnimState::ATTACK:
			animator.PlayOnce(attackClip);
			break;
		default:
			//Emote State
			animator.PlayOnce(emoteClip);
			break;
		}

	}


	void CSkullFSM::Update()
	{
		auto& animator = m_owner->Get<CSpriteAnimator>();

		//TODO: Complete this function.
		switch (m_state)
		{
		case AnimState::IDLE:

			if (GetTrigger("skullAttack"))
				SetState(AnimState::ATTACK);

			if (GetTrigger("skullEmote"))
				SetState(AnimState::EMOTE);

			break;
		case AnimState::ATTACK:

			if (animator.IsDone())
				SetState(AnimState::IDLE);

			break;
		default:
			//Emote State
			if (animator.IsDone())
				SetState(AnimState::IDLE);

			break;
		}
	}
}