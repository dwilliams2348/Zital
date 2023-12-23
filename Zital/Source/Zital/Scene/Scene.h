#pragma once

#include "Zital/Core/TimeStep.h"

#include "entt.hpp"

namespace Zital
{

	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		//temp
		entt::registry& Reg() { return mRegistry; }

		void OnUpdate(Timestep _deltaTime);

	private:
		entt::registry mRegistry;
	};

}