#pragma once

#include "Scene.h"

#include "entt.hpp"

namespace Zital
{

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity _handle, Scene* _scene);
		Entity(const Entity& _other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&...args) 
		{ 
			ZT_CORE_ASSERT(!HasComponent<T>(), "Component already exists on this entity.");
			return mScene->mRegistry.emplace<T>(mEntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent() 
		{ 
			ZT_CORE_ASSERT(HasComponent<T>(), "Component doesn't exist on this entity.");
			return mScene->mRegistry.get<T>(mEntityHandle); 
		}

		template<typename T>
		bool HasComponent() { return mScene->mRegistry.any_of<T>(mEntityHandle); }

		template<typename T>
		void RemoveComponent() 
		{ 
			ZT_CORE_ASSERT(HasComponent<T>(), "Component doesn't exist on this entity.");
			mScene->mRegistry.remove<T>(mEntityHandle);
		}

		operator bool() const { return mEntityHandle != entt::null; }

	private:
		entt::entity mEntityHandle{ entt::null };
		Scene* mScene = nullptr;
	};

}