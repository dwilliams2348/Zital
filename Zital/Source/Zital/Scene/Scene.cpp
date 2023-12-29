#include "ZTpch.h"
#include "Scene.h"

#include "Components.h"
#include "Zital/Renderer/Renderer2D.h"

#include "Entity.h"

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

	Entity Scene::CreateEntity(const std::string& _name)
	{
		Entity entity = { mRegistry.create(), this };
		auto& tag = entity.AddComponent<TagComponent>(_name);
		tag.Tag = _name.empty() ? "Entity" : _name;

		return entity;
	}

	void Scene::OnUpdate(Timestep _deltaTime)
	{
		//update scripts
		{
			mRegistry.view<NativeScriptComponent>().each([&](auto _entity, auto& _script)
				{
					if (!_script.Instance)
					{
						_script.InstantiateFunction();
						_script.Instance->mEntity = { _entity, this };

						if(_script.OnCreateFunction)
							_script.OnCreateFunction(_script.Instance);
					}

					if(_script.OnUpdateFunction)
						_script.OnUpdateFunction(_script.Instance, _deltaTime);
				});
		}

		//render 2D scene
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = mRegistry.view<CameraComponent, TransformComponent>();
			for (auto entity : view)
			{
				auto& [camera, transform] = view.get<CameraComponent, TransformComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;

					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, *cameraTransform);

			auto group = mRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform, sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t _width, uint32_t _height)
	{
		mViewportWidth = _width;
		mViewportHeight = _height;

		//resize our non fixed aspect ratio cameras
		auto view = mRegistry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(_width, _height);
		}
	}

}