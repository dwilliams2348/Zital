#include "ZTpch.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include "Zital/Core/Timer.h"

namespace Zital
{

	namespace Utils
	{

		static GLenum ShaderTypeFromString(const std::string& _type)
		{
			if (_type == "vertex") { return GL_VERTEX_SHADER; }
			if (_type == "fragment" || _type == "pixel") { return GL_FRAGMENT_SHADER; }

			ZT_CORE_ASSERT(false, "Unknown shader type.");
			return 0;
		}

		static shaderc_shader_kind GLShaderStageToShaderC(GLenum _stage)
		{
			switch (_stage)
			{
				case GL_VERTEX_SHADER:		return shaderc_glsl_vertex_shader;
				case GL_FRAGMENT_SHADER:	return shaderc_glsl_fragment_shader;
			}

			ZT_CORE_ASSERT(false, "Invalid shader type");
			return (shaderc_shader_kind)0;
		}

		static const char* GLShaderStageToString(GLenum _stage)
		{
			switch (_stage)
			{
				case GL_VERTEX_SHADER:		return "GL_VERTEX_SHADER";
				case GL_FRAGMENT_SHADER:	return "GL_FRAGMENT_SHADER";
			}

			ZT_CORE_ASSERT(false, "Invalid shader type");
			return nullptr;
		}

		static const char* GetCachedDirectory()
		{
			//TODO make sure that the dir is valid
			return "Assets/Cache/Shader/OpenGL";
		}

		static void CreateCacheDirectoryIfNeeded()
		{
			std::string cacheDirectory = GetCachedDirectory();
			if (!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}

		static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t _stage)
		{
			switch (_stage)
			{
				case GL_VERTEX_SHADER:		return ".cached_opengl.vert";
				case GL_FRAGMENT_SHADER:	return ".cached_opengl.frag";
			}

			ZT_CORE_ASSERT(false, "Invalid shader type");
			return "";
		}

		static const char* GLShaderStageCachedVulkanFileExtension(uint32_t _stage)
		{
			switch (_stage)
			{
				case GL_VERTEX_SHADER:		return ".cached_vulkan.vert";
				case GL_FRAGMENT_SHADER:	return ".cached_vulkan.frag";
			}

			ZT_CORE_ASSERT(false, "Invalid shader type");
			return "";
		}

	}

	OpenGLShader::OpenGLShader(const std::string& _filepath)
		: mFilePath(_filepath)
	{
		ZT_PROFILE_FUNCTION();

		Utils::CreateCacheDirectoryIfNeeded();

		std::string shaderSource = ReadFile(_filepath);
		auto shaderSources = PreProcess(shaderSource);

		{
			Timer timer;
			CompileOrGetVulkanBinaries(shaderSources);
			CompileOrGetOpenGLBinaries();
			CreateProgram();
			ZT_CORE_WARN("Shader creation took {0} ms", timer.ElapsedMillis());
		}

		//extracting name from filepath
		auto lastSlash = _filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;

		auto lastDot = _filepath.rfind('.');
		auto count = lastDot == std::string::npos ? _filepath.size() - lastSlash : lastDot - lastSlash;

		mName = _filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& _name, const std::string& _vertexSource, const std::string& _fragmentSource)
		: mName(_name)
	{
		ZT_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = _vertexSource;
		sources[GL_FRAGMENT_SHADER] = _fragmentSource;

		CompileOrGetVulkanBinaries(sources);
		CompileOrGetOpenGLBinaries();
		CreateProgram();
	}

	OpenGLShader::~OpenGLShader()
	{
		ZT_PROFILE_FUNCTION();

		glDeleteProgram(mRendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& _filepath)
	{
		ZT_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(_filepath, std::ios::in | std::ios::binary);

		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else { ZT_CORE_ERROR("Could not open file '{0}'", _filepath); }

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& _source)
	{
		ZT_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = _source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			//find end of file line
			size_t eol = _source.find_first_of("\r\n", pos);
			ZT_CORE_ASSERT(eol != std::string::npos, "Syntax Error");
			//find beginning of the line
			size_t begin = pos + typeTokenLength + 1;
			std::string type = _source.substr(begin, eol - begin);
			ZT_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type specified ");

			size_t nextLinePos = _source.find_first_not_of("\r\n", eol);
			pos = _source.find(typeToken, nextLinePos);
			//add type and then source code to unordered map for return
			shaderSources[Utils::ShaderTypeFromString(type)] = _source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? _source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& _shaderSources)
	{
		GLuint program = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCachedDirectory();

		auto& shaderData = mVulkanSPIRV;
		shaderData.clear();
		for (auto&& [stage, source] : _shaderSources)
		{
			std::filesystem::path shaderFilePath = mFilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), mFilePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					ZT_CORE_ERROR(module.GetErrorMessage());
					ZT_CORE_ASSERT(false);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto&& [stage, data] : shaderData)
			Reflect(stage, data);
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		auto& shaderData = mOpenGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

		const bool optimize = false;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCachedDirectory();

		shaderData.clear();
		mOpenGLSourceCode.clear();
		for (auto&& [stage, spirv] : mVulkanSPIRV)
		{
			std::filesystem::path shaderFilePath = mFilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				mOpenGLSourceCode[stage] = glslCompiler.compile();
				auto& source = mOpenGLSourceCode[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), mFilePath.c_str());
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					ZT_CORE_ERROR(module.GetErrorMessage());
					ZT_CORE_ASSERT(false, "Shader compilation filed");
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	void OpenGLShader::CreateProgram()
	{
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : mOpenGLSPIRV)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			ZT_CORE_ERROR("Shader linking failed ({0}):\n{1}", mFilePath, infoLog.data());

			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		mRendererID = program;
	}

	void OpenGLShader::Reflect(GLenum _stage, const std::vector<uint32_t>& _shaderData)
	{
		spirv_cross::Compiler compiler(_shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		ZT_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(_stage), mFilePath);
		ZT_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
		ZT_CORE_TRACE("    {0} resources", resources.sampled_images.size());

		ZT_CORE_TRACE("Uniform buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();

			ZT_CORE_TRACE("  {0}", resource.name);
			ZT_CORE_TRACE("    Size = {0}", bufferSize);
			ZT_CORE_TRACE("    Binding = {0}", binding);
			ZT_CORE_TRACE("    Members = {0}", memberCount);
		}
	}

	void OpenGLShader::Bind() const
	{
		ZT_PROFILE_FUNCTION();

		glUseProgram(mRendererID);
	}

	void OpenGLShader::Unbind() const
	{
		ZT_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& _name, int _value)
	{
		ZT_PROFILE_FUNCTION();

		UploadUniformInt(_name, _value);
	}

	void OpenGLShader::SetIntArray(const std::string& _name, int* _values, uint32_t _count)
	{
		ZT_PROFILE_FUNCTION();

		UploadUniformIntArray(_name, _values, _count);
	}

	void OpenGLShader::SetFloat(const std::string& _name, float _value)
	{
		ZT_PROFILE_FUNCTION();

		UploadUniformFloat(_name, _value);
	}

	void OpenGLShader::SetFloat3(const std::string& _name, const glm::vec3& _value)
	{
		ZT_PROFILE_FUNCTION();

		UploadUniformFloat3(_name, _value);
	}

	void OpenGLShader::SetFloat4(const std::string& _name, const glm::vec4& _value)
	{
		ZT_PROFILE_FUNCTION();

		UploadUniformFloat4(_name, _value);
	}

	void OpenGLShader::SetMat4(const std::string& _name, const glm::mat4& _value)
	{
		ZT_PROFILE_FUNCTION();

		UploadUniformMat4(_name, _value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& _name, int _value)
	{
		GLint location = glGetUniformLocation(mRendererID, _name.c_str());
		glUniform1i(location, _value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& _name, int* _values, uint32_t _count)
	{
		GLint location = glGetUniformLocation(mRendererID, _name.c_str());
		glUniform1iv(location, _count, _values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& _name, float _value)
	{
		GLint location = glGetUniformLocation(mRendererID, _name.c_str());
		glUniform1f(location, _value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& _name, const glm::vec2& _value)
	{
		GLint location = glGetUniformLocation(mRendererID, _name.c_str());
		glUniform2f(location, _value.x, _value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& _name, const glm::vec3& _value)
	{
		GLint location = glGetUniformLocation(mRendererID, _name.c_str());
		glUniform3f(location, _value.x, _value.y, _value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& _name, const glm::vec4& _value)
	{
		GLint location = glGetUniformLocation(mRendererID, _name.c_str());
		glUniform4f(location, _value.x, _value.y, _value.z, _value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& _name, const glm::mat3& _matrix)
	{
		GLint location = glGetUniformLocation(mRendererID, _name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(_matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& _name, const glm::mat4& _matrix)
	{
		GLint location = glGetUniformLocation(mRendererID, _name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(_matrix));
	}

}