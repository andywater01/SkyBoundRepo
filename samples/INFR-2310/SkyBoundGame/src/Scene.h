#pragma once
#include "entt.hpp"

namespace Midterm {

	class Scene
	{
	public:
		Scene();
		~Scene();
	private:
		entt::registry m_Registry;

	};

}
