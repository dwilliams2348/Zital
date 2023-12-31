#pragma once

#include "Zital/Renderer/OrthographicCamera.h"
#include "Zital/Renderer/Camera.h"
#include "Zital/Renderer/EditorCamera.h"

#include "Zital/Renderer/Texture.h"
#include "Zital/Renderer/SubTexture2D.h"

#include "Zital/Scene/Components.h"

namespace Zital
{

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& _camera, const glm::mat4& _transform);
		static void BeginScene(const EditorCamera& _camera);
		static void BeginScene(const OrthographicCamera& _camera);//remove at some point
		static void EndScene();
		static void Flush();
		

		//Primitives
		//Draws filled in quad using a 4x4 transform amtrix
		static void DrawQuad(const glm::mat4& _transform, const glm::vec4 _color, int _entityID = -1);
		//Draws textured quad using a 4x4 transform matrix
		static void DrawQuad(const glm::mat4& _transform, const Ref<Texture2D>& _texture, float _tilingFactor = 1.f, const glm::vec4& _tintColor = glm::vec4(1.f), int _entityID = -1);
		//Draws textured quad using a 4x4 transform matrix and subtexture
		static void DrawQuad(const glm::mat4& _transform, const Ref<SubTexture2D>& _subTexture, float _tilingFactor = 1.f, const glm::vec4& _tintColor = glm::vec4(1.f), int _entityID = -1);

		//Draws filled in quad using x, y position with a size and color
		static void DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const glm::vec4& _color);
		//Draws filled in quad using x, y, z position with a size and color
		static void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color);
		//Draws textured quad using x, y position with a size
		static void DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture, float _tilingFactor = 1.f, const glm::vec4& _tintColor = glm::vec4(1.f));
		//Draws textured quad using x, y, z position with a size
		static void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture, float _tilingFactor = 1.f, const glm::vec4& _tintColor = glm::vec4(1.f));
		//Draws textured quad using x, y position with a size and subtexture
		static void DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const Ref<SubTexture2D>& _subTexture, float _tilingFactor = 1.f, const glm::vec4& _tintColor = glm::vec4(1.f));
		//Draws textured quad using x, y, z position with a size and subtexture
		static void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<SubTexture2D>& _subTexture, float _tilingFactor = 1.f, const glm::vec4& _tintColor = glm::vec4(1.f));

		//Draws filled in quad using x, y position with a size, rotation, and color
		static void DrawRotatedQuad(const glm::vec2& _position, const glm::vec2& _size, float _radians, const glm::vec4& _color);
		//Draws filled in quad using x, y, z position with a size, rotation, and color
		static void DrawRotatedQuad(const glm::vec3& _position, const glm::vec2& _size, float _radians, const glm::vec4& _color);
		//Draws textured quad using x, y position with a size and rotation
		static void DrawRotatedQuad(const glm::vec2& _position, const glm::vec2& _size, float _radians, const Ref<Texture2D>& _texture, float _tilingFactor = 1.f, const glm::vec4& _tintColor = glm::vec4(1.f));
		//Draws textured quad using x, y, z position with a size and rotatoin
		static void DrawRotatedQuad(const glm::vec3& _position, const glm::vec2& _size, float _radians, const Ref<Texture2D>& _texture, float _tilingFactor = 1.f, const glm::vec4& _tintColor = glm::vec4(1.f));
		//Draws textured quad using x, y position with a size, a rotation and subtexture
		static void DrawRotatedQuad(const glm::vec2& _position, const glm::vec2& _size, float _radians, const Ref<SubTexture2D>& _subTexture, float _tilingFactor = 1.f, const glm::vec4& _tintColor = glm::vec4(1.f));
		//Draws textured quad using x, y, z position with a size, a rotation and subtexture
		static void DrawRotatedQuad(const glm::vec3& _position, const glm::vec2& _size, float _radians, const Ref<SubTexture2D>& _subTexture, float _tilingFactor = 1.f, const glm::vec4& _tintColor = glm::vec4(1.f));

		//Draws a sprite with transform and sprite component
		static void DrawSprite(const glm::mat4& _transform, SpriteRendererComponent& _sprite, int _entityID);

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
		static void StartBatch();
		static void NextBatch();
	};

}