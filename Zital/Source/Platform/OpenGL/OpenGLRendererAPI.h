#pragma once

#include "Zital/Renderer/RendererAPI.h"

namespace Zital
{

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& _color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& _vertexArray) override;
	};

}