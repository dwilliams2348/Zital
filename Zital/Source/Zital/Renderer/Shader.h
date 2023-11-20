#pragma once

#include <string>

namespace Zital
{

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(const std::string& _vertexSource, const std::string& _fragmentSource);

	};

}