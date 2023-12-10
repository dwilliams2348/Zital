#pragma once

#include "RendererAPI.h"

namespace Zital
{

	class RenderCommand
	{
	public:
		inline static void Init()
		{
			sRendererAPI->Init();
		}

		inline static void SetViewport(uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height)
		{
			sRendererAPI->SetViewport(_x, _y, _width, _height);
		}

		inline static void SetClearColor(const glm::vec4& _color)
		{
			sRendererAPI->SetClearColor(_color);
		}

		inline static void Clear()
		{
			sRendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& _vertexArray)
		{
			sRendererAPI->DrawIndexed(_vertexArray);
		}

	private:
		static RendererAPI* sRendererAPI;
	};

}