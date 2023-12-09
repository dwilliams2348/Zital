#include "ZTpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Zital
{
	Ref<Shader> Shader::Create(const std::string& _filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	ZT_CORE_ASSERT(false, "RendererAPI::None is currently not supported.");	return nullptr;
		case RendererAPI::API::OpenGL:	return Ref<Shader>(new OpenGLShader(_filepath));
		}

		ZT_CORE_ASSERT(false, "Unknown RendererAPI.");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& _vertexSource, const std::string& _fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	ZT_CORE_ASSERT(false, "RendererAPI::None is currently not supported.");	return nullptr;
			case RendererAPI::API::OpenGL:	return Ref<Shader>(new OpenGLShader(_vertexSource, _fragmentSource));
		}

		ZT_CORE_ASSERT(false, "Unknown RendererAPI.");
		return nullptr;
	}

}