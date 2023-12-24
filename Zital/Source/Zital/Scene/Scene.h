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

		void OnUpdate(Timestep _deltaTime);

	private:
		entt::registry mRegistry;

		friend class Entity;
	};

}