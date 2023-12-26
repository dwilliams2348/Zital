#pragma once

#include "Zital/Renderer/Camera.h"

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
		Zital::Camera Camera;
		bool Primary = true; // move to scene at some point

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& _projection)
			: Camera(_projection) {}
	};

}