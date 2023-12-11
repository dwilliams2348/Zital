#include "ZTpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Zital
{

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* sData;

	void Renderer2D::Init()
	{
		ZT_PROFILE_FUNCTION();

		sData = new Renderer2DStorage();

		sData->QuadVertexArray = VertexArray::Create();

		float squareVertices[5 * 4] =
		{
			-0.5f, -0.5f, 0.f, 0.f, 0.f,
			 0.5f, -0.5f, 0.f, 1.f, 0.f,
			 0.5f,  0.5f, 0.f, 1.f, 1.f,
			-0.5f,  0.5f, 0.f, 0.f, 1.f
		};

		Ref<VertexBuffer> SquareVB;
		SquareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		SquareVB->SetLayout({
			{ShaderDataType::Float3, "aPosition"},
			{ShaderDataType::Float2, "aTexCoord"}
			});

		sData->QuadVertexArray->AddVertexBuffer(SquareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

		sData->QuadVertexArray->SetIndexBuffer(squareIB);

		sData->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTexData = 0xFFFFFFFF;
		sData->WhiteTexture->SetData(&whiteTexData, sizeof(whiteTexData));

		sData->TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		sData->TextureShader->Bind();
		sData->TextureShader->SetInt("uTexture", 0);
	}

	void Renderer2D::Shutdown()
	{
		ZT_PROFILE_FUNCTION();

		delete sData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& _camera)
	{
		ZT_PROFILE_FUNCTION();

		sData->TextureShader->Bind();
		sData->TextureShader->SetMat4("uViewProjection", _camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		ZT_PROFILE_FUNCTION();


	}

	void Renderer2D::DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const glm::vec4& _color)
	{
		DrawQuad({ _position.x, _position.y, 0.f }, _size, _color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color)
	{
		ZT_PROFILE_FUNCTION();

		sData->TextureShader->Bind();
		sData->TextureShader->SetFloat4("uColor", _color);
		sData->TextureShader->SetFloat("uTiliingFactor", 1.f);
		sData->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.f), _position) *
			glm::scale(glm::mat4(1.f), {_size.x, _size.y, 1.f});
		sData->TextureShader->SetMat4("uTransform", transform);

		sData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(sData->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const Ref<Texture>& _texture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		DrawQuad({ _position.x, _position.y, 0.f }, _size, _texture, _tilingFactor, _tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture>& _texture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		ZT_PROFILE_FUNCTION();

		sData->TextureShader->Bind();

		sData->TextureShader->SetFloat4("uColor", _tintColor);
		sData->TextureShader->SetFloat("uTilingFactor", _tilingFactor);
		_texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.f), _position) * 
			glm::scale(glm::mat4(1.f), { _size.x, _size.y, 1.f });

		sData->TextureShader->SetMat4("uTransform", transform);

		sData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(sData->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& _position, const glm::vec2& _size, float _radians, const glm::vec4& _color)
	{
		DrawRotatedQuad({ _position.x, _position.y, 0.f }, _size, _radians, _color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& _position, const glm::vec2& _size, float _radians, const glm::vec4& _color)
	{
		ZT_PROFILE_FUNCTION();

		sData->TextureShader->Bind();
		sData->TextureShader->SetFloat4("uColor", _color);
		sData->TextureShader->SetFloat("uTiliingFactor", 1.f);
		sData->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.f), _position) * /*add rotation next*/
			glm::rotate(glm::mat4(1.f), _radians, { 0.f, 0.f, 1.f }) *
			glm::scale(glm::mat4(1.f), { _size.x, _size.y, 1.f });
		sData->TextureShader->SetMat4("uTransform", transform);

		sData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(sData->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& _position, const glm::vec2& _size, float _radians, const Ref<Texture>& _texture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		DrawRotatedQuad({ _position.x, _position.y, 0.f }, _size, _radians, _texture, _tilingFactor, _tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& _position, const glm::vec2& _size, float _radians, const Ref<Texture>& _texture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		ZT_PROFILE_FUNCTION();

		sData->TextureShader->Bind();

		sData->TextureShader->SetFloat4("uColor", _tintColor);
		sData->TextureShader->SetFloat("uTilingFactor", _tilingFactor);
		_texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.f), _position) * /*add rotation next*/
			glm::rotate(glm::mat4(1.f), _radians, { 0.f, 0.f, 1.f }) *
			glm::scale(glm::mat4(1.f), { _size.x, _size.y, 1.f });

		sData->TextureShader->SetMat4("uTransform", transform);

		sData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(sData->QuadVertexArray);
	}

}