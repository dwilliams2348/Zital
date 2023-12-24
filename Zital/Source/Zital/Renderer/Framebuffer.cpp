#include "ZTpch.h"
#include "Framebuffer.h"

#include "Zital/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Zital
{

	Ref<Framebuffer> Framebuffer::Create(const FramebufferProperties& _props)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	ZT_CORE_ASSERT(false, "RendererAPI::None is currently not supported.");	return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLFramebuffer>(_props);
		}

		ZT_CORE_ASSERT(false, "Unknown RendererAPI.");
		return nullptr;
	}

}