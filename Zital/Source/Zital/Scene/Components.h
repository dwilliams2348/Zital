#pragma once

#include "Zital/Scene/SceneCamera.h"
#include "Zital/Scene/ScriptableEntity.h"

#include <glm/glm.hpp>

namespace Zital
{

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& _tag)
			: Tag(_tag) {}
	};

	struct TransformComponent
	{
		glm::mat4 Transform = glm::mat4(1.f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& _transform)
			: Transform(_transform){}

		operator glm::mat4&() { return Transform; }
		operator const glm::mat4&() const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.f, 1.f, 1.f, 1.f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& _color)
			: Color(_color) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; // move to scene at some point
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		std::function<void()> InstantiateFunction;
		std::function<void()> DestroyInstanceFunction;

		std::function<void(ScriptableEntity*)> OnCreateFunction;
		std::function<void(ScriptableEntity*)> OnDestroyFunction;
		std::function<void(ScriptableEntity*, Timestep)> OnUpdateFunction;

		template <typename T>
		void Bind()
		{
			InstantiateFunction = [&]() { Instance = new T(); };
			DestroyInstanceFunction = [&]() { delete (T*)Instance; Instance = nullptr; };

			OnCreateFunction = [](ScriptableEntity* _instance) { ((T*)_instance)->OnCreate(); };
			OnDestroyFunction = [](ScriptableEntity* _instance) { ((T*)_instance)->OnDestroy(); };
			OnUpdateFunction = [](ScriptableEntity* _instance, Timestep _deltaTime) { ((T*)_instance)->OnUpdate(_deltaTime); };
		}
	};

}