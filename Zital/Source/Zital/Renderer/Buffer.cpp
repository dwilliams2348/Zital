#include "ZTpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Zital
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t _size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	ZT_CORE_ASSERT(false, "RendererAPI::None is currently not supported in the Zital engine."); return nullptr;
			case RendererAPI::API::OpenGL:	return Ref<VertexBuffer>(new OpenGLVertexBuffer(_size));
		}

		ZT_CORE_ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* _vertices, uint32_t _size)
	{
		switch (Renderer::GetAPI()) 
		{
			case RendererAPI::API::None:	ZT_CORE_ASSERT(false, "RendererAPI::None is currently not supported in the Zital engine."); return nullptr;
			case RendererAPI::API::OpenGL:	return Ref<VertexBuffer>(new OpenGLVertexBuffer(_vertices, _size));
		}

		ZT_CORE_ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* _indices, uint32_t _count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	ZT_CORE_ASSERT(false, "RendererAPI::None is currently not supported in the Zital engine."); return nullptr;
			case RendererAPI::API::OpenGL:	return Ref<IndexBuffer>(new OpenGLIndexBuffer(_indices, _count));
		}

		ZT_CORE_ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}

}