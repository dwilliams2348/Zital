#include "ZTpch.h"
#include "Renderer2D.h"

#include "Zital/Renderer/VertexArray.h"
#include "Zital/Renderer/Shader.h"
#include "Zital/Renderer/UniformBuffer.h"
#include "Zital/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

		//Editor only attribs
		int EntityID;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 100000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
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

		Renderer2D::Statistics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer2DData sData;

	void Renderer2D::Init()
	{
		ZT_PROFILE_FUNCTION();

		sData.QuadVertexArray = VertexArray::Create();

		sData.QuadVertexBuffer = VertexBuffer::Create(sData.MaxVertices * sizeof(QuadVertex));

		sData.QuadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "aPosition"     },
			{ShaderDataType::Float4, "aColor"        },
			{ShaderDataType::Float2, "aTexCoord"     },
			{ShaderDataType::Float,  "aTexIndex"     },
			{ShaderDataType::Float,  "aTilingFactor" },
			{ShaderDataType::Int,    "aEntityID"     }
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

		sData.TextureSlots[0] = sData.WhiteTexture;

		sData.QuadVertexPositions[0] = { -0.5, -0.5, 0.f, 1.f };
		sData.QuadVertexPositions[1] = {  0.5, -0.5, 0.f, 1.f };
		sData.QuadVertexPositions[2] = {  0.5,  0.5, 0.f, 1.f };
		sData.QuadVertexPositions[3] = { -0.5,  0.5, 0.f, 1.f };

		sData.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer2D::Shutdown()
	{
		ZT_PROFILE_FUNCTION();

		delete[] sData.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Camera& _camera, const glm::mat4& _transform)
	{
		ZT_PROFILE_FUNCTION();

		sData.CameraBuffer.ViewProjection = _camera.GetProjection() * glm::inverse(_transform);
		sData.CameraUniformBuffer->SetData(&sData.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& _camera)
	{
		ZT_PROFILE_FUNCTION();

		sData.CameraBuffer.ViewProjection = _camera.GetViewProjection();
		sData.CameraUniformBuffer->SetData(&sData.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& _camera)
	{
		ZT_PROFILE_FUNCTION();

		sData.TextureShader->Bind();
		sData.TextureShader->SetMat4("uViewProjection", _camera.GetViewProjectionMatrix());

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		ZT_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::StartBatch()
	{
		sData.QuadIndexCount = 0;
		sData.QuadVertexBufferPtr = sData.QuadVertexBufferBase;

		sData.TextureSlotIndex = 1;
	}

	void Renderer2D::Flush()
	{
		if (sData.QuadIndexCount == 0)
			return; //nothing to draw at this time

		uint32_t dataSize = (uint32_t)((uint8_t*)sData.QuadVertexBufferPtr - (uint8_t*)sData.QuadVertexBufferBase);
		sData.QuadVertexBuffer->SetData(sData.QuadVertexBufferBase, dataSize);

		//bind textures
		for (uint32_t i = 0; i < sData.TextureSlotIndex; i++)
			sData.TextureSlots[i]->Bind(i);

		sData.TextureShader->Bind();
		RenderCommand::DrawIndexed(sData.QuadVertexArray, sData.QuadIndexCount);

		sData.Stats.DrawCalls++;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::DrawQuad(const glm::mat4& _transform, const glm::vec4 _color, int _entityID)
	{
		ZT_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		glm::vec2 texCoords[] = {
			{ 0.f, 0.f },
			{ 1.f, 0.f },
			{ 1.f, 1.f },
			{ 0.f, 1.f }
		};

		constexpr float textureIndex = 0.f; //white texture
		constexpr float tilingFactor = 1.f;

		if (sData.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		for (int i = 0; i < quadVertexCount; i++)
		{
			sData.QuadVertexBufferPtr->Position = _transform * sData.QuadVertexPositions[i];
			sData.QuadVertexBufferPtr->Color = _color;
			sData.QuadVertexBufferPtr->TexCoord = texCoords[i];
			sData.QuadVertexBufferPtr->TexIndex = textureIndex;
			sData.QuadVertexBufferPtr->TiliingFactor = tilingFactor;
			sData.QuadVertexBufferPtr->EntityID = _entityID;
			sData.QuadVertexBufferPtr++;
		}

		sData.QuadIndexCount += 6;

		sData.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& _transform, const Ref<Texture2D>& _texture, float _tilingFactor, const glm::vec4& _tintColor, int _entityID)
	{
		ZT_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		glm::vec2 texCoords[] = {
			{ 0.f, 0.f },
			{ 1.f, 0.f },
			{ 1.f, 1.f },
			{ 0.f, 1.f }
		};

		float textureIndex = 0.f;

		if (sData.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

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

		for (int i = 0; i < quadVertexCount; i++)
		{
			sData.QuadVertexBufferPtr->Position = _transform * sData.QuadVertexPositions[i];
			sData.QuadVertexBufferPtr->Color = _tintColor;
			sData.QuadVertexBufferPtr->TexCoord = texCoords[i];
			sData.QuadVertexBufferPtr->TexIndex = textureIndex;
			sData.QuadVertexBufferPtr->TiliingFactor = _tilingFactor;
			sData.QuadVertexBufferPtr->EntityID = _entityID;
			sData.QuadVertexBufferPtr++;
		}

		sData.QuadIndexCount += 6;

		sData.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& _transform, const Ref<SubTexture2D>& _subTexture, float _tilingFactor, const glm::vec4& _tintColor, int _entityID)
	{
		ZT_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const glm::vec2* texCoords = _subTexture->GetTexCoords();

		float textureIndex = 0.f;

		if (sData.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		for (uint32_t i = 1; i < sData.TextureSlotIndex; i++)
		{
			if (*sData.TextureSlots[i].get() == *_subTexture->GetTexture().get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.f)
		{
			textureIndex = (float)sData.TextureSlotIndex;
			sData.TextureSlots[sData.TextureSlotIndex] = _subTexture->GetTexture();
			sData.TextureSlotIndex++;
		}

		for (int i = 0; i < quadVertexCount; i++)
		{
			sData.QuadVertexBufferPtr->Position = _transform * sData.QuadVertexPositions[i];
			sData.QuadVertexBufferPtr->Color = _tintColor;
			sData.QuadVertexBufferPtr->TexCoord = texCoords[i];
			sData.QuadVertexBufferPtr->TexIndex = textureIndex;
			sData.QuadVertexBufferPtr->TiliingFactor = _tilingFactor;
			sData.QuadVertexBufferPtr->EntityID = _entityID;
			sData.QuadVertexBufferPtr++;
		}

		sData.QuadIndexCount += 6;

		sData.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const glm::vec4& _color)
	{
		DrawQuad({ _position.x, _position.y, 0.f }, _size, _color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), _position) *
			glm::scale(glm::mat4(1.f), { _size.x, _size.y, 1.f });

		DrawQuad(transform, _color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		DrawQuad({ _position.x, _position.y, 0.f }, _size, _texture, _tilingFactor, _tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), _position) *
			glm::scale(glm::mat4(1.f), { _size.x, _size.y, 1.f });

		DrawQuad(transform, _texture, _tilingFactor, _tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const Ref<SubTexture2D>& _subTexture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		DrawQuad({ _position.x, _position.y, 0.f }, _size, _subTexture, _tilingFactor, _tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<SubTexture2D>& _subTexture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), _position) *
			glm::scale(glm::mat4(1.f), { _size.x, _size.y, 1.f });

		DrawQuad(transform, _subTexture, _tilingFactor, _tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& _position, const glm::vec2& _size, float _radians, const glm::vec4& _color)
	{
		DrawRotatedQuad({ _position.x, _position.y, 0.f }, _size, _radians, _color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& _position, const glm::vec2& _size, float _radians, const glm::vec4& _color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), _position) *
			glm::rotate(glm::mat4(1.f), _radians, { 0.f, 0.f, 1.f }) *
			glm::scale(glm::mat4(1.f), { _size.x, _size.y, 1.f });

		DrawQuad(transform, _color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& _position, const glm::vec2& _size, float _radians, const Ref<Texture2D>& _texture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		DrawRotatedQuad({ _position.x, _position.y, 0.f }, _size, _radians, _texture, _tilingFactor, _tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& _position, const glm::vec2& _size, float _radians, const Ref<Texture2D>& _texture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), _position) *
			glm::rotate(glm::mat4(1.f), _radians, { 0.f, 0.f, 1.f }) *
			glm::scale(glm::mat4(1.f), { _size.x, _size.y, 1.f });

		DrawQuad(transform, _texture, _tilingFactor, _tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& _position, const glm::vec2& _size, float _radians, const Ref<SubTexture2D>& _subTexture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		DrawRotatedQuad({ _position.x, _position.y, 0.f }, _size, _radians, _subTexture, _tilingFactor, _tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& _position, const glm::vec2& _size, float _radians, const Ref<SubTexture2D>& _subTexture, float _tilingFactor, const glm::vec4& _tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), _position) *
			glm::rotate(glm::mat4(1.f), _radians, { 0.f, 0.f, 1.f }) *
			glm::scale(glm::mat4(1.f), { _size.x, _size.y, 1.f });

		DrawQuad(transform, _subTexture, _tilingFactor, _tintColor);
	}

	void Renderer2D::DrawSprite(const glm::mat4& _transform, SpriteRendererComponent& _sprite, int _entityID)
	{
		DrawQuad(_transform, _sprite.Color, _entityID);
	}

	void Renderer2D::ResetStats()
	{
		memset(&sData.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics& Renderer2D::GetStats()
	{
		return sData.Stats;
	}
}