#pragma once

#include "Transform.h"

#include "entt.hpp"


class Entity
{
public:

	Transform transform;

	static Entity Create();
	static std::unique_ptr<Entity> Allocate();

	Entity(entt::entity id);
	Entity(Entity&&) = delete;

	virtual ~Entity();

	template<typename T, typename... Args>
	T& Add(Args&&... args)
	{
		return ecs.emplace<T>(m_id, std::forward<Args>(args)...);
	}

	template<typename T>
	T& Get()
	{
		return ecs.get<T>(m_id);
	}

	template<typename T>
	void Remove()
	{
		ecs.remove<T>(m_id);
	}

protected:

	static entt::registry ecs;
	entt::entity m_id;
};
