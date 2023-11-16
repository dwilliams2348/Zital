#include "ZTpch.h"
#include "Renderer.h"

namespace Zital
{

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& _vertexArray)
	{
		_vertexArray->Bind();
		RenderCommand::DrawIndexed(_vertexArray);
	}

}