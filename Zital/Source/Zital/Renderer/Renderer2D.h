#pragma once

#include "OrthographicCamera.h"

namespace Zital
{

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& _camera);
		static void EndScene();

		//Primitives
		//Draws filled in quad using x, y position with a size and color
		static void DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const glm::vec4& _color);
		//Draws filled in quad using x, y, z position with a size and color
		static void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color);

	};

}