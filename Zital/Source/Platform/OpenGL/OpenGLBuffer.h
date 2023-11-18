#pragma once

#include "Zital/Renderer/Buffer.h"

namespace Zital
{

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* _vertices, uint32_t _size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return mLayout; }
		virtual void SetLayout(const BufferLayout& _layout) override { mLayout = _layout; }

	private:
		uint32_t mRendererID;
		BufferLayout mLayout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* _indices, uint32_t _count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetIndexCount() const { return mIndexCount; }
	private:
		uint32_t mRendererID;
		uint32_t mIndexCount;
	};

}