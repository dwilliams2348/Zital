#include "ZTpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Zital
{

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		//add tex ID, mask ID? eventially
	};

	struct Renderer2DData
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
	};

	static Renderer2DData sData;

	void Renderer2D::Init()
	{
		ZT_PROFILE_FUNCTION();

		sData.QuadVertexArray = VertexArray::Create();

		sData.QuadVertexBuffer = VertexBuffer::Create(sData.MaxVertices * sizeof(QuadVertex));

		sData.QuadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "aPosition"},
			{ShaderDataType::Float4, "aColor"},
			{ShaderDataType::Float2, "aTexCoord"}
			});

		sData.QuadVertexArray->AddVertexBuffer(sData.QuadVertexBuffer);

		sData.QuadVertexBufferBase = new QuadVertex[sData.MaxVertices];

		uint32_t* quadIndices = new uint32_t[sData.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < sData.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, sData.MaxIndices);
		sData.QuadVertexArray->SetIndexBuffer(quadIB);

		delete[] quadIndices;

		sData.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTexData = 0xFFFFFFFF;
		sData.WhiteTexture->SetData(&whiteTexData, sizeof(whiteTexData));

		sData.TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		sData.TextureShader->Bind();
		sData.TextureShader->SetInt("uTexture", 0);
	}

	void Renderer2D::Shutdown()
	{
		ZT_PROFILE_FUNCTION();

	}

	void Renderer2D::BeginScene(const OrthographicCamera& _camera)
	{
		ZT_PROFILE_FUNCTION();

		sData.TextureShader->Bind();
		sData.TextureShader->SetMat4("uViewProjection", _camera.GetViewProjectionMatrix());

		sData.QuadIndexCount = 0;
		sData.QuadVertexBufferPtr = sData.QuadVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{
		ZT_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)sData.QuadVertexBufferPtr - (uint8_t*)sData.QuadVertexBufferBase;
		sData.QuadVertexBuffer->SetData(sData.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndexed(sData.QuadVertexArray, sData.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const glm::vec4& _color)
	{
		DrawQuad({ _position.x, _position.y, 0.f }, _size, _color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color)
	{
		ZT_PROFILE_FUNCTION();

		sData.QuadVertexBufferPtr->Position = _position;
		sData.QuadVertexBufferPtr->Color = _color;
		sData.QuadVertexBufferPtr->TexCoord = { 0.f, 0.f };
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = { _position.x + _size.x, _position.y, _position.z };
		sData.QuadVertexBufferPtr->Color = _color;
		sData.QuadVertexBufferPtr->TexCoord = { 1.f, 0.f };
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = { _position.x + _size.x, _position.y + _size.y, _position.z };
		sData.QuadVertexBufferPtr->Color = _color;
		sData.QuadVertexBufferPtr->TexCoord = { 1.f, 1.f };
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = { _position.x , _position.y + _size.y, _position.z };
		sData.QuadVertexBufferPtr->Color = _color;
		sData.QuadVertexBufferPtr->TexCoord = { 0.f, 1.f };
		sData.QuadVertexBufferPtr++;

		sData.QuadIndexCount += 6;

		/*sData.TextureShader->Bind();
		sData.TextureShader->SetFloat4("uColor", _color);
		sData.TextureShader->SetFloat("uTiliingFactor", 1.f);
		sData.WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.f), _position) *
			glm::scale(glm::mat4(1.f), {_size.x, _size.y, 1.f});
		sData.TextureShader->SetMat4("uTransform", transform);

		sData.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(sData.QuadVertexArray);*/
	}

	void Renderer2D::DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const Ref<Texture>& _texture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		DrawQuad({ _position.x, _position.y, 0.f }, _size, _texture, _tilingFactor, _tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture>& _texture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		ZT_PROFILE_FUNCTION();

		sData.TextureShader->Bind();

		sData.TextureShader->SetFloat4("uColor", _tintColor);
		sData.TextureShader->SetFloat("uTilingFactor", _tilingFactor);
		_texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.f), _position) * 
			glm::scale(glm::mat4(1.f), { _size.x, _size.y, 1.f });

		sData.TextureShader->SetMat4("uTransform", transform);

		sData.QuadVertexArray->Bind();
		//RenderCommand::DrawIndexed(sData.QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& _position, const glm::vec2& _size, float _radians, const glm::vec4& _color)
	{
		DrawRotatedQuad({ _position.x, _position.y, 0.f }, _size, _radians, _color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& _position, const glm::vec2& _size, float _radians, const glm::vec4& _color)
	{
		ZT_PROFILE_FUNCTION();

		sData.TextureShader->Bind();
		sData.TextureShader->SetFloat4("uColor", _color);
		sData.TextureShader->SetFloat("uTiliingFactor", 1.f);
		sData.WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.f), _position) * /*add rotation next*/
			glm::rotate(glm::mat4(1.f), _radians, { 0.f, 0.f, 1.f }) *
			glm::scale(glm::mat4(1.f), { _size.x, _size.y, 1.f });
		sData.TextureShader->SetMat4("uTransform", transform);

		sData.QuadVertexArray->Bind();
		//RenderCommand::DrawIndexed(sData.QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& _position, const glm::vec2& _size, float _radians, const Ref<Texture>& _texture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		DrawRotatedQuad({ _position.x, _position.y, 0.f }, _size, _radians, _texture, _tilingFactor, _tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& _position, const glm::vec2& _size, float _radians, const Ref<Texture>& _texture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		ZT_PROFILE_FUNCTION();

		sData.TextureShader->Bind();

		sData.TextureShader->SetFloat4("uColor", _tintColor);
		sData.TextureShader->SetFloat("uTilingFactor", _tilingFactor);
		_texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.f), _position) * /*add rotation next*/
			glm::rotate(glm::mat4(1.f), _radians, { 0.f, 0.f, 1.f }) *
			glm::scale(glm::mat4(1.f), { _size.x, _size.y, 1.f });

		sData.TextureShader->SetMat4("uTransform", transform);

		sData.QuadVertexArray->Bind();
		//RenderCommand::DrawIndexed(sData.QuadVertexArray);
	}

}