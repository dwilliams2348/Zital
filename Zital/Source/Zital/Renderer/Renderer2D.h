#pragma once

#include "OrthographicCamera.h"

#include "Texture.h"

namespace Zital
{

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& _camera);
		static void EndScene();
		static void Flush();

		//Primitives
		//Draws filled in quad using x, y position with a size and color
		static void DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const glm::vec4& _color);
		//Draws filled in quad using x, y, z position with a size and color
		static void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color);
		//Draws textured quad using x, y position with a size
		static void DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture, float _tilingFactor = 1.f, const glm::vec4& _tintColor = glm::vec4(1.f));
		//Draws textured quad using x, y, z position with a size
		static void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture, float _tilingFactor = 1.f, const glm::vec4& _tintColor = glm::vec4(1.f));

		//Draws filled in quad using x, y position with a size, rotation, and color
		static void DrawRotatedQuad(const glm::vec2& _position, const glm::vec2& _size, float _degrees, const glm::vec4& _color);
		//Draws filled in quad using x, y, z position with a size, rotation, and color
		static void DrawRotatedQuad(const glm::vec3& _position, const glm::vec2& _size, float _degrees, const glm::vec4& _color);
		//Draws textured quad using x, y position with a size and rotation
		static void DrawRotatedQuad(const glm::vec2& _position, const glm::vec2& _size, float _degrees, const Ref<Texture2D>& _texture, float _tilingFactor = 1.f, const glm::vec4 & _tintColor = glm::vec4(1.f));
		//Draws textured quad using x, y, z position with a size and rotatoin
		static void DrawRotatedQuad(const glm::vec3& _position, const glm::vec2& _size, float _degrees, const Ref<Texture2D>& _texture, float _tilingFactor = 1.f, const glm::vec4& _tintColor = glm::vec4(1.f));

		//Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics& GetStats();

	private:
		static void FlushAndReset();
	};

}