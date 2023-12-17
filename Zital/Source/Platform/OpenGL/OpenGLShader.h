#pragma once

#include "Zital/Renderer/Shader.h"

#include <glm/glm.hpp>

//TODO remove and properly cast in sandbox
typedef unsigned int GLenum;

namespace Zital
{

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& _filepath);
		OpenGLShader(const std::string& _name, const std::string& _vertexSource, const std::string& _fragmentSource);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string& _name, int _value) override;
		virtual void SetIntArray(const std::string& _name, int* _values, uint32_t _count) override;
		virtual void SetFloat(const std::string& _name, float _value) override;
		virtual void SetFloat3(const std::string& _name, const glm::vec3& _value) override;
		virtual void SetFloat4(const std::string& _name, const glm::vec4& _value) override;
		virtual void SetMat4(const std::string& _name, const glm::mat4& _value) override;

		virtual const std::string& GetName() const override { return mName; }

		void UploadUniformInt(const std::string& _name, int _value);
		void UploadUniformIntArray(const std::string& _name, int* _values, uint32_t _count);
			   
		void UploadUniformFloat(const std::string& _name, float _value);
		void UploadUniformFloat2(const std::string& _name, const glm::vec2& _value);
		void UploadUniformFloat3(const std::string& _name, const glm::vec3& _value);
		void UploadUniformFloat4(const std::string& _name, const glm::vec4& _value);
			   
		void UploadUniformMat3(const std::string& _name, const glm::mat3& _matrix);
		void UploadUniformMat4(const std::string& _name, const glm::mat4& _matrix);

	private:
		std::string ReadFile(const std::string& _filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& _source);
		void Compile(const std::unordered_map<GLenum, std::string>& _shaderSources);

	private:
		uint32_t mRendererID;
		std::string mName;

	};

}