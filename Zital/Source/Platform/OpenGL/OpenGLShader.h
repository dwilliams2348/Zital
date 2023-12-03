#pragma once

#include "Zital/Renderer/Shader.h"

#include <glm/glm.hpp>

namespace Zital
{

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& _vertexSource, const std::string& _fragmentSource);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UpdateUniformInt(const std::string& _name, int _value);

		void UpdateUniformFloat(const std::string& _name, float _value);
		void UpdateUniformFloat2(const std::string& _name, const glm::vec2& _value);
		void UpdateUniformFloat3(const std::string& _name, const glm::vec3& _value);
		void UpdateUniformFloat4(const std::string& _name, const glm::vec4& _value);

		void UpdateUniformMat3(const std::string& _name, const glm::mat3& _matrix);
		void UpdateUniformMat4(const std::string& _name, const glm::mat4& _matrix);

	private:
		uint32_t mRendererID;

	};

}