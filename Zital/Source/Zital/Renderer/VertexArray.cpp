#include "ZTpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Zital
{

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	ZT_CORE_ASSERT(false, "RendererAPI::None is currently not supported."); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexArray>();
		}
	}

}