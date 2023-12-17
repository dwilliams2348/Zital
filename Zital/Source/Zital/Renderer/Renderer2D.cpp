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
		float TexIndex;
		float TiliingFactor;
		//mask ID? eventually
	};

	struct Renderer2DData
	{
		const uint32_t MaxQuads = 100000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTexSlots = 32; //render capabilities

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTexSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; //0, the first texture slot will always be set to the constant white texture for non textured rendering

		glm::vec4 QuadVertexPositions[4];
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
			{ShaderDataType::Float2, "aTexCoord"},
			{ShaderDataType::Float, "aTexIndex"},
			{ShaderDataType::Float, "aTilingFactor"}
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

		int32_t samplers[sData.MaxTexSlots];
		for (uint32_t i = 0; i < sData.MaxTexSlots; i++)
			samplers[i] = i;

		sData.TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		sData.TextureShader->Bind();
		sData.TextureShader->SetIntArray("uTextures", samplers, sData.MaxTexSlots);

		sData.TextureSlots[0] = sData.WhiteTexture;

		sData.QuadVertexPositions[0] = { -0.5, -0.5, 0.f, 1.f };
		sData.QuadVertexPositions[1] = {  0.5, -0.5, 0.f, 1.f };
		sData.QuadVertexPositions[2] = {  0.5,  0.5, 0.f, 1.f };
		sData.QuadVertexPositions[3] = { -0.5,  0.5, 0.f, 1.f };
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

		sData.TextureSlotIndex = 1;
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
		//bind textures
		for (uint32_t i = 0; i < sData.TextureSlotIndex; i++)
			sData.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(sData.QuadVertexArray, sData.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const glm::vec4& _color)
	{
		DrawQuad({ _position.x, _position.y, 0.f }, _size, _color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color)
	{
		ZT_PROFILE_FUNCTION();

		const float texIndex = 0.f; //white texture
		const float tilingFactor = 1.f;

		glm::mat4 transform = glm::translate(glm::mat4(1.f), _position) *
			glm::scale(glm::mat4(1.f), { _size.x, _size.y, 1.f });

		sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[0];
		sData.QuadVertexBufferPtr->Color = _color;
		sData.QuadVertexBufferPtr->TexCoord = { 0.f, 0.f };
		sData.QuadVertexBufferPtr->TexIndex = texIndex;
		sData.QuadVertexBufferPtr->TiliingFactor = tilingFactor;
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[1];
		sData.QuadVertexBufferPtr->Color = _color;
		sData.QuadVertexBufferPtr->TexCoord = { 1.f, 0.f };
		sData.QuadVertexBufferPtr->TexIndex = texIndex;
		sData.QuadVertexBufferPtr->TiliingFactor = tilingFactor;
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[2];
		sData.QuadVertexBufferPtr->Color = _color;
		sData.QuadVertexBufferPtr->TexCoord = { 1.f, 1.f };
		sData.QuadVertexBufferPtr->TexIndex = texIndex;
		sData.QuadVertexBufferPtr->TiliingFactor = tilingFactor;
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[3];
		sData.QuadVertexBufferPtr->Color = _color;
		sData.QuadVertexBufferPtr->TexCoord = { 0.f, 1.f };
		sData.QuadVertexBufferPtr->TexIndex = texIndex;
		sData.QuadVertexBufferPtr->TiliingFactor = tilingFactor;
		sData.QuadVertexBufferPtr++;

		sData.QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		DrawQuad({ _position.x, _position.y, 0.f }, _size, _texture, _tilingFactor, _tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		ZT_PROFILE_FUNCTION();

		float textureIndex = 0.f;

		for (uint32_t i = 1; i < sData.TextureSlotIndex; i++)
		{
			if (*sData.TextureSlots[i].get() == *_texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.f)
		{
			textureIndex = (float)sData.TextureSlotIndex;
			sData.TextureSlots[sData.TextureSlotIndex] = _texture;
			sData.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.f), _position) *
			glm::scale(glm::mat4(1.f), { _size.x, _size.y, 1.f });

		sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[0];
		sData.QuadVertexBufferPtr->Color = _tintColor;
		sData.QuadVertexBufferPtr->TexCoord = { 0.f, 0.f };
		sData.QuadVertexBufferPtr->TexIndex = textureIndex;
		sData.QuadVertexBufferPtr->TiliingFactor = _tilingFactor;
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[1];
		sData.QuadVertexBufferPtr->Color = _tintColor;
		sData.QuadVertexBufferPtr->TexCoord = { 1.f, 0.f };
		sData.QuadVertexBufferPtr->TexIndex = textureIndex;
		sData.QuadVertexBufferPtr->TiliingFactor = _tilingFactor;
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[2];
		sData.QuadVertexBufferPtr->Color = _tintColor;
		sData.QuadVertexBufferPtr->TexCoord = { 1.f, 1.f };
		sData.QuadVertexBufferPtr->TexIndex = textureIndex;
		sData.QuadVertexBufferPtr->TiliingFactor = _tilingFactor;
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[3];
		sData.QuadVertexBufferPtr->Color = _tintColor;
		sData.QuadVertexBufferPtr->TexCoord = { 0.f, 1.f };
		sData.QuadVertexBufferPtr->TexIndex = textureIndex;
		sData.QuadVertexBufferPtr->TiliingFactor = _tilingFactor;
		sData.QuadVertexBufferPtr++;

		sData.QuadIndexCount += 6;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& _position, const glm::vec2& _size, float _degrees, const glm::vec4& _color)
	{
		DrawRotatedQuad({ _position.x, _position.y, 0.f }, _size, _degrees, _color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& _position, const glm::vec2& _size, float _degrees, const glm::vec4& _color)
	{
		ZT_PROFILE_FUNCTION();

		const float textureIndex = 0.f;
		const float tilingFactor = 1.f;

		glm::mat4 transform = glm::translate(glm::mat4(1.f), _position) *
			glm::rotate(glm::mat4(1.f), glm::radians(_degrees), { 0.f, 0.f, 1.f }) *
			glm::scale(glm::mat4(1.f), { _size.x, _size.y, 1.f });

		sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[0];
		sData.QuadVertexBufferPtr->Color = _color;
		sData.QuadVertexBufferPtr->TexCoord = { 0.f, 0.f };
		sData.QuadVertexBufferPtr->TexIndex = textureIndex;
		sData.QuadVertexBufferPtr->TiliingFactor = tilingFactor;
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[1];
		sData.QuadVertexBufferPtr->Color = _color;
		sData.QuadVertexBufferPtr->TexCoord = { 1.f, 0.f };
		sData.QuadVertexBufferPtr->TexIndex = textureIndex;
		sData.QuadVertexBufferPtr->TiliingFactor = tilingFactor;
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[2];
		sData.QuadVertexBufferPtr->Color = _color;
		sData.QuadVertexBufferPtr->TexCoord = { 1.f, 1.f };
		sData.QuadVertexBufferPtr->TexIndex = textureIndex;
		sData.QuadVertexBufferPtr->TiliingFactor = tilingFactor;
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[3];
		sData.QuadVertexBufferPtr->Color = _color;
		sData.QuadVertexBufferPtr->TexCoord = { 0.f, 1.f };
		sData.QuadVertexBufferPtr->TexIndex = textureIndex;
		sData.QuadVertexBufferPtr->TiliingFactor = tilingFactor;
		sData.QuadVertexBufferPtr++;

		sData.QuadIndexCount += 6;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& _position, const glm::vec2& _size, float _degrees, const Ref<Texture2D>& _texture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		DrawRotatedQuad({ _position.x, _position.y, 0.f }, _size, _degrees, _texture, _tilingFactor, _tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& _position, const glm::vec2& _size, float _degrees, const Ref<Texture2D>& _texture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		ZT_PROFILE_FUNCTION();

		float textureIndex = 0.f;

		for (uint32_t i = 1; i < sData.TextureSlotIndex; i++)
		{
			if (*sData.TextureSlots[i].get() == *_texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.f)
		{
			textureIndex = (float)sData.TextureSlotIndex;
			sData.TextureSlots[sData.TextureSlotIndex] = _texture;
			sData.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.f), _position) *
			glm::rotate(glm::mat4(1.f), glm::radians(_degrees), { 0.f, 0.f, 1.f }) *
			glm::scale(glm::mat4(1.f), { _size.x, _size.y, 1.f });

		sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[0];
		sData.QuadVertexBufferPtr->Color = _tintColor;
		sData.QuadVertexBufferPtr->TexCoord = { 0.f, 0.f };
		sData.QuadVertexBufferPtr->TexIndex = textureIndex;
		sData.QuadVertexBufferPtr->TiliingFactor = _tilingFactor;
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[1];
		sData.QuadVertexBufferPtr->Color = _tintColor;
		sData.QuadVertexBufferPtr->TexCoord = { 1.f, 0.f };
		sData.QuadVertexBufferPtr->TexIndex = textureIndex;
		sData.QuadVertexBufferPtr->TiliingFactor = _tilingFactor;
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[2];
		sData.QuadVertexBufferPtr->Color = _tintColor;
		sData.QuadVertexBufferPtr->TexCoord = { 1.f, 1.f };
		sData.QuadVertexBufferPtr->TexIndex = textureIndex;
		sData.QuadVertexBufferPtr->TiliingFactor = _tilingFactor;
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[3];
		sData.QuadVertexBufferPtr->Color = _tintColor;
		sData.QuadVertexBufferPtr->TexCoord = { 0.f, 1.f };
		sData.QuadVertexBufferPtr->TexIndex = textureIndex;
		sData.QuadVertexBufferPtr->TiliingFactor = _tilingFactor;
		sData.QuadVertexBufferPtr++;

		sData.QuadIndexCount += 6;
	}

}