#include "ZTpch.h"
#include "Scene.h"

#include "Components.h"
#include "Zital/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Zital
{

	Scene::Scene()
	{
		//struct TransformComponent
		//{
		//	glm::mat4 Transform;

		//	TransformComponent() = default;
		//	TransformComponent(const TransformComponent&) = default;
		//	TransformComponent(const glm::mat4& _transform)
		//		: Transform(_transform){}

		//	operator glm::mat4&() { return Transform; }
		//	operator const glm::mat4&() { return Transform; }
		//};

		//struct MeshComponent
		//{
		//	float value;
		//};

		//entt::entity entity = mRegistry.create();

		////put entity on component
		//mRegistry.emplace<TransformComponent>(entity, glm::mat4(1.f));
		////retrieves component from entity
		//TransformComponent& transform = mRegistry.get<TransformComponent>(entity);
		////removes component from entity
		//mRegistry.remove<TransformComponent>(entity);

		////creates view that has all entities with specified component
		//auto view = mRegistry.view<TransformComponent>();
		//for (auto entity : view)
		//{
		//	//do something
		//	TransformComponent& transform = view.get<TransformComponent>(entity);
		//}

		////creates group of mulitple components, similar to view
		//auto group = mRegistry.group<TransformComponent>(entt::get<MeshComponent>);
		//for (auto entity : group)
		//{
		//	auto&[transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		//}
	}

	Scene::~Scene()
	{

	}

	entt::entity Scene::CreateEntity()
	{
		return mRegistry.create();
	}

	void Scene::OnUpdate(Timestep _deltaTime)
	{
		auto group = mRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}

}