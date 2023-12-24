#include "ZTpch.h"
#include "Entity.h"

namespace Zital
{

	Entity::Entity(entt::entity _handle, Scene* _scene)
		: mEntityHandle(_handle), mScene(_scene)
	{
	}

}