#include "ZTpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Zital
{

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		ZT_CORE_ASSERT(false, "RendererAPI::None is currently not supported."); return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLVertexArray();
		}
	}

}