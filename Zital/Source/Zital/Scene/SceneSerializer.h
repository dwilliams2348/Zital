#pragma once

#include "Zital/Scene/Scene.h"

namespace Zital
{

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& _scene);

		void Serialize(const std::string& _filepath);
		void SerializeRuntime(const std::string& _filepath);
		
		bool Deserialize(const std::string& _filepath);
		bool DeserializeRuntime(const std::string& _filepath);

	private:
		Ref<Scene> mScene;
	};

}