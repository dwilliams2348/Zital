#include "ZTpch.h"
#include "Zital/Scene/SceneSerializer.h"

#include "Zital/Scene/Entity.h"
#include "Zital/Scene/Components.h"

#include <yaml-cpp/yaml.h>

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Zital
{

	YAML::Emitter& operator<<(YAML::Emitter& _out, const glm::vec3& v)
	{
		_out << YAML::Flow;
		_out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;

		return _out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& _out, const glm::vec4& v)
	{
		_out << YAML::Flow;
		_out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;

		return _out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& _scene)
		: mScene(_scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& _out, Entity _entity)
	{
		_out << YAML::BeginMap; //entity
		_out << YAML::Key << "Entity" << YAML::Value << "13265495374552"; //entity ID will go here

		if (_entity.HasComponent<TagComponent>())
		{
			_out << YAML::Key << "TagComponent";
			_out << YAML::BeginMap; //tag component

			auto& tag = _entity.GetComponent<TagComponent>().Tag;
			_out << YAML::Key << "Tag" << YAML::Value << tag;

			_out << YAML::EndMap; //tag component
		}

		if (_entity.HasComponent<TransformComponent>())
		{
			_out << YAML::Key << "TransformComponent";
			_out << YAML::BeginMap; //transform component

			auto& transform = _entity.GetComponent<TransformComponent>();
			_out << YAML::Key << "Translation" << YAML::Value << transform.Translation;
			_out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
			_out << YAML::Key << "Scale" << YAML::Value << transform.Scale;

			_out << YAML::EndMap; //transform component
		}

		if (_entity.HasComponent<CameraComponent>())
		{
			_out << YAML::Key << "CameraComponent";
			_out << YAML::BeginMap; //camera component

			auto& cameraComponent = _entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			_out << YAML::Key << "Camera" << YAML::Value;
			_out << YAML::BeginMap; //camera
			_out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			_out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			_out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			_out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			_out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			_out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			_out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			_out << YAML::EndMap; //camera

			_out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			_out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			_out << YAML::EndMap; //camera component
		}

		if (_entity.HasComponent<SpriteRendererComponent>())
		{
			_out << YAML::Key << "SpriteRendererComponent";
			_out << YAML::BeginMap; //sprite renderer component

			auto& sprite = _entity.GetComponent<SpriteRendererComponent>();
			_out << YAML::Key << "Color" << YAML::Value << sprite.Color;

			_out << YAML::EndMap; //sprite renderer component
		}

		_out << YAML::EndMap; //entity
	}

	void SceneSerializer::Serialize(const std::string& _filepath)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		mScene->mRegistry.each([&](auto _entityID)
			{
				Entity entity = { _entityID, mScene.get() };
				if (!entity)
					return;

				SerializeEntity(out, entity);
			});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(_filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& _filepath)
	{
		ZT_CORE_ASSERT(false, "Not Implemented");
	}

	bool SceneSerializer::Deserialize(const std::string& _filepath)
	{
		std::ifstream stream(_filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		ZT_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];

		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>(); //TODO

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				ZT_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = mScene->CreateEntity(name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& transform = deserializedEntity.AddComponent<TransformComponent>();
					transform.Translation = transformComponent["Translation"].as<glm::vec3>();
					transform.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					transform.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& camera = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];
					camera.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					camera.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					camera.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					camera.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					camera.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					camera.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					camera.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					camera.Primary = cameraComponent["Primary"].as<bool>();
					camera.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& sprite = deserializedEntity.AddComponent<SpriteRendererComponent>();
					sprite.Color = spriteRendererComponent["Color"].as<glm::vec4>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& _filepath)
	{
		ZT_CORE_ASSERT(false, "Not Implemented");

		return false;
	}

}