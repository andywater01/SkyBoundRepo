#include "Scene.h"
#include "GLM/glm.hpp"

namespace Midterm {

	static void DoMath(const glm::mat4& transform)
	{

	}

	Scene::Scene() 
	{
		struct MeshComponent {
		
			float data;
			MeshComponent() = default;

		};

		struct TransformComponent
		{
			glm::mat4 Transform;

			TransformComponent() = default;
			TransformComponent(const TransformComponent&) = default;
			TransformComponent(const glm::mat4& transform)
				: Transform(transform) {}

			operator glm::mat4& () { return Transform; }
			operator const glm::mat4&() const { return Transform; }
		};

		TransformComponent transform;
		DoMath(transform);


		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		m_Registry.remove<TransformComponent>(entity);

		if (m_Registry.has<TransformComponent>(entity))
		{
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
		}


		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
		}


		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			auto&[transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		}

	}

	Scene::~Scene()
	{

	}

}
