#include "ZTpch.h"
#include "UniformBuffer.h"

#include "Zital/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Zital
{

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t _size, uint32_t _binding)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: ZT_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLUniformBuffer>(_size, _binding);
		}

		ZT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}
