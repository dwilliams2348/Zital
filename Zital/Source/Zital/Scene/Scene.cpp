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

	void Scene::DestroyEntity(Entity _entity)
	{
		mRegistry.destroy(_entity);
	}

	void Scene::OnUpdateRuntime(Timestep _deltaTime)
	{
		//update scripts
		{
			mRegistry.view<NativeScriptComponent>().each([&](auto _entity, auto& _script)
				{
					//move to Scene::OnScenePlay
					if (!_script.Instance)
					{
						_script.Instance = _script.InstantiateScript();
						_script.Instance->mEntity = { _entity, this };

						
						_script.Instance->OnCreate();
					}

					
					_script.Instance->OnUpdate(_deltaTime);
				});
		}

		//render 2D scene
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = mRegistry.view<CameraComponent, TransformComponent>();
			for (auto entity : view)
			{
				auto [camera, transform] = view.get<CameraComponent, TransformComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();

					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto group = mRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnUpdateEditor(Timestep _deltaTime, EditorCamera& _camera)
	{
		Renderer2D::BeginScene(_camera);

		auto group = mRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}

		Renderer2D::EndScene();
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

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = mRegistry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}

		return Entity{};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity _entity, T& _component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity _entity, TagComponent& _component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity _entity, TransformComponent& _component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity _entity, CameraComponent& _component)
	{
		_component.Camera.SetViewportSize(mViewportWidth, mViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity _entity, SpriteRendererComponent& _component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity _entity, NativeScriptComponent& _component)
	{
	}

}