#pragma once

#include "RendererAPI.h"

namespace Zital
{

	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& _color)
		{
			sRendererAPI->SetClearColor(_color);
		}

		inline static void Clear()
		{
			sRendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& _vertexArray)
		{
			sRendererAPI->DrawIndexed(_vertexArray);
		}

	private:
		static RendererAPI* sRendererAPI;
	};

}