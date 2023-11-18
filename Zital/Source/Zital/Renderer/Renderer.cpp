#include "ZTpch.h"
#include "Renderer.h"

namespace Zital
{

	Renderer::SceneData* Renderer::mSceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& _camera)
	{
		mSceneData->ViewProjectionMatrix = _camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& _shader, const std::shared_ptr<VertexArray>& _vertexArray)
	{
		_shader->Bind();
		_shader->UpdateUniformMat4("uViewProjection", mSceneData->ViewProjectionMatrix);

		_vertexArray->Bind();
		RenderCommand::DrawIndexed(_vertexArray);
	}

}