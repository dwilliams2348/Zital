#include "ZTpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Zital
{

	static GLenum ShaderTypeFromString(const std::string& _type)
	{
		if (_type == "vertex") { return GL_VERTEX_SHADER; }
		if (_type == "fragment" || _type == "pixel") { return GL_FRAGMENT_SHADER; }

		ZT_CORE_ASSERT(false, "Unknown shader type.");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& _filepath)
	{
		std::string shaderSource = ReadFile(_filepath);
		auto shaderSources = PreProcess(shaderSource);
		Compile(shaderSources);

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
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = _vertexSource;
		sources[GL_FRAGMENT_SHADER] = _fragmentSource;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(mRendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& _filepath)
	{
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
			shaderSources[ShaderTypeFromString(type)] = _source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? _source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& _shaderSources)
	{
		// Get a program object.
		GLuint program = glCreateProgram();
		ZT_CORE_ASSERT(_shaderSources.size() <= 2, "Only upto 2 shader types are supported at this time.");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIndex = 0;
		
		for (auto& keyVal : _shaderSources)
		{
			GLenum type = keyVal.first;
			const std::string& source = keyVal.second;

			// Create an empty vertex shader handle
			GLuint shader = glCreateShader(type);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				ZT_CORE_ERROR("{0}", infoLog.data());
				ZT_CORE_ASSERT(false, "Shader compilation has failed.");
				break;
			}

			//attach the shaders
			glAttachShader(program, shader);
			glShaderIDs[glShaderIndex++] = shader;
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			// Don't leak shaders either.
			for (auto id : glShaderIDs)
				glDeleteShader(id);

			ZT_CORE_ERROR("{0}", infoLog.data());
			ZT_CORE_ASSERT(false, "Shader linking has failed.");
			return;
		}

		// Always detach shaders after a successful link.
		for(auto id : glShaderIDs)
			glDetachShader(program, id);

		mRendererID = program;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(mRendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetFloat3(const std::string& _name, const glm::vec3& _value)
	{
		UploadUniformFloat3(_name, _value);
	}

	void OpenGLShader::SetFloat4(const std::string& _name, const glm::vec4& _value)
	{
		UploadUniformFloat4(_name, _value);
	}

	void OpenGLShader::SetMat4(const std::string& _name, const glm::mat4& _value)
	{
		UploadUniformMat4(_name, _value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& _name, int _value)
	{
		GLint location = glGetUniformLocation(mRendererID, _name.c_str());
		glUniform1i(location, _value);
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