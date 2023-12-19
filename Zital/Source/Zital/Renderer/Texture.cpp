#include "ZTpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Zital
{
	Ref<Texture2D> Texture2D::Create(uint32_t _width, uint32_t _height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	ZT_CORE_ASSERT(false, "RendererAPI::None is currently not supported."); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(_width, _height);
		}
	}

	Ref<Texture2D> Texture2D::Create(const std::string& _filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	ZT_CORE_ASSERT(false, "RendererAPI::None is currently not supported."); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(_filepath);
		}
	}

}