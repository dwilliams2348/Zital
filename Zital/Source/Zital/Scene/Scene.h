#pragma once

#include "Zital/Core/TimeStep.h"

#include "entt.hpp"

namespace Zital
{

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& _name = std::string());
		void DestroyEntity(Entity _entity);

		void OnUpdate(Timestep _deltaTime);
		void OnViewportResize(uint32_t _width, uint32_t _height);

		Entity GetPrimaryCameraEntity();

	private:
		template<typename T>
		void OnComponentAdded(Entity _entity, T& _component);

	private:
		entt::registry mRegistry;

		uint32_t mViewportWidth = 0, mViewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}