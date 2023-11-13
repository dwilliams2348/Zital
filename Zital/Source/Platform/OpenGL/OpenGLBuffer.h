#pragma once

#include "Zital/Renderer/Buffer.h"

namespace Zital
{

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* _vertices, uint32_t _size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;
	private:
		uint32_t mRendererID;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* _indices, uint32_t _count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetIndexCount() const { return mIndexCount; }
	private:
		uint32_t mRendererID;
		uint32_t mIndexCount;
	};

}