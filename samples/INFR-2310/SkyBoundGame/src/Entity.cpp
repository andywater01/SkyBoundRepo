#include "Entity.h"

entt::registry Entity::ecs;

Entity Entity::Create()
{
	entt::entity id = ecs.create();
	return Entity(id);
}

std::unique_ptr<Entity> Entity::Allocate()
{
	entt::entity id = ecs.create();
	return std::move(std::make_unique<Entity>(id));
}

Entity::Entity(entt::entity id)
{
	m_id = id;
}

Entity::~Entity()
{
	if (m_id != entt::null)
		ecs.destroy(m_id);
}