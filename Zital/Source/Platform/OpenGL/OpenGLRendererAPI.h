#pragma once

#include "Zital/Renderer/RendererAPI.h"

namespace Zital
{

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height) override;

		virtual void SetClearColor(const glm::vec4& _color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& _vertexArray) override;
	};

}