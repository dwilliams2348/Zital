#include "ZTpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Zital
{

	VertexBuffer* VertexBuffer::Create(float* _vertices, uint32_t _size)
	{
		switch (Renderer::GetAPI()) 
		{
			case RendererAPI::None:		ZT_CORE_ASSERT(false, "RendererAPI::None is currently not supported in the Zital engine."); return nullptr;
			case RendererAPI::OpenGL:	return new OpenGLVertexBuffer(_vertices, _size);
		}

		ZT_CORE_ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* _indices, uint32_t _count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:		ZT_CORE_ASSERT(false, "RendererAPI::None is currently not supported in the Zital engine."); return nullptr;
			case RendererAPI::OpenGL:	return new OpenGLIndexBuffer(_indices, _count);
		}

		ZT_CORE_ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}

}