#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"

namespace Zital
{

	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& _camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& _shader, const Ref<VertexArray>& _vertexArray, const glm::mat4& _transform = glm::mat4(1.f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* mSceneData;
	};

}