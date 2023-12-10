#include "ZTpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Zital
{

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorage* sData;

	void Renderer2D::Init()
	{
		sData = new Renderer2DStorage();

		sData->QuadVertexArray = VertexArray::Create();

		float squareVertices[3 * 4] =
		{
			-0.5f, -0.5f, 0.f,
			 0.5f, -0.5f, 0.f,
			 0.5f,  0.5f, 0.f,
			-0.5f,  0.5f, 0.f
		};

		Ref<VertexBuffer> SquareVB;
		SquareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		SquareVB->SetLayout({
			{ShaderDataType::Float3, "aPosition"}
			});

		sData->QuadVertexArray->AddVertexBuffer(SquareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

		sData->QuadVertexArray->SetIndexBuffer(squareIB);

		sData->FlatColorShader = Shader::Create("Assets/Shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete sData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& _camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(sData->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(sData->FlatColorShader)->UpdateUniformMat4("uViewProjection", _camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(sData->FlatColorShader)->UpdateUniformMat4("uTransform", glm::mat4(1.f));
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const glm::vec4& _color)
	{
		DrawQuad({ _position.x, _position.y, 0.f }, _size, _color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(sData->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(sData->FlatColorShader)->UpdateUniformFloat4("uColor", _color);

		sData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(sData->QuadVertexArray);
	}

}