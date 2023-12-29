#pragma once

#include "Zital/Scene/Entity.h"

namespace Zital
{

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent() { return mEntity.GetComponent<T>(); }

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep _deltaTime) {}

	private:
		Entity mEntity;

		friend class Scene;
	};

}