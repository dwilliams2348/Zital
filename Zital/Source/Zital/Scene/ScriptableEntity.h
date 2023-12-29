#pragma once

#include "Zital/Scene/Entity.h"

namespace Zital
{

	class ScriptableEntity
	{
	public:
		template<typename T>
		T& GetComponent() { return mEntity.GetComponent<T>(); }

	private:
		Entity mEntity;

		friend class Scene;
	};

}