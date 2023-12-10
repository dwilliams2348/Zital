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

	Ref<Shader> Shader::Create(const std::string& _name, const std::string& _vertexSource, const std::string& _fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	ZT_CORE_ASSERT(false, "RendererAPI::None is currently not supported.");	return nullptr;
			case RendererAPI::API::OpenGL:	return Ref<Shader>(new OpenGLShader(_name, _vertexSource, _fragmentSource));
		}

		ZT_CORE_ASSERT(false, "Unknown RendererAPI.");
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& _name, const Ref<Shader>& _shader)
	{
		ZT_CORE_ASSERT(!Exists(_name), "Shader already exists, cannot add again.");
		mShaders[_name] = _shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& _shader)
	{
		auto& name = _shader->GetName();
		Add(name, _shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& _filepath)
	{
		auto shader = Shader::Create(_filepath);
		Add(shader);

		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& _name, const std::string& _filepath)
	{
		auto shader = Shader::Create(_filepath);
		Add(_name, shader);

		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& _name)
	{
		ZT_CORE_ASSERT(Exists(_name), "Could not find the specified shader.");
		return mShaders[_name];
	}

	bool ShaderLibrary::Exists(const std::string& _name) const
	{
		return mShaders.find(_name) != mShaders.end();
	}

}