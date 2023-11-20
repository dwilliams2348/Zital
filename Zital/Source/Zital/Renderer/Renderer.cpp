#include "ZTpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

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

	void Renderer::Submit(const std::shared_ptr<Shader>& _shader, const std::shared_ptr<VertexArray>& _vertexArray, const glm::mat4& _transform)
	{
		_shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(_shader)->UpdateUniformMat4("uViewProjection", mSceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(_shader)->UpdateUniformMat4("uTransform", _transform);

		_vertexArray->Bind();
		RenderCommand::DrawIndexed(_vertexArray);
	}

}