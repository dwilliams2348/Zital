#include "ZTpch.h"
#include "Renderer.h"

#include "Renderer2D.h"

namespace Zital
{

	Scope<Renderer::SceneData> Renderer::mSceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		ZT_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		ZT_PROFILE_FUNCTION();

		Renderer2D::Shutdown();
	}

    void Renderer::OnWindowResize(uint32_t _width, uint32_t _height)
    {
		RenderCommand::SetViewport(0, 0, _width, _height);
    }

	void Renderer::BeginScene(OrthographicCamera& _camera)
	{
		mSceneData->ViewProjectionMatrix = _camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& _shader, const Ref<VertexArray>& _vertexArray, const glm::mat4& _transform)
	{
		_shader->Bind();
		_shader->SetMat4("uViewProjection", mSceneData->ViewProjectionMatrix);
		_shader->SetMat4("uTransform", _transform);

		_vertexArray->Bind();
		RenderCommand::DrawIndexed(_vertexArray);
	}

}