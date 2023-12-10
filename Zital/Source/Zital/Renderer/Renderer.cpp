#include "ZTpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer2D.h"

namespace Zital
{

	Renderer::SceneData* Renderer::mSceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
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
		std::dynamic_pointer_cast<OpenGLShader>(_shader)->UpdateUniformMat4("uViewProjection", mSceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(_shader)->UpdateUniformMat4("uTransform", _transform);

		_vertexArray->Bind();
		RenderCommand::DrawIndexed(_vertexArray);
	}

}