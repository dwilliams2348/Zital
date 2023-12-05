#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"

namespace Zital
{

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, //headless or when rendering not needed
			OpenGL
		};

	public:
		virtual void Init() = 0;

		virtual void SetClearColor(const glm::vec4& _color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& _vertexArray) = 0;

		inline static API GetAPI() { return sAPI; }

	private:
		static API sAPI;
	};

}