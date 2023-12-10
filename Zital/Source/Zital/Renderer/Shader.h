#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Zital
{

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		
		virtual void SetInt(const std::string& _name, int _value) = 0;
		virtual void SetFloat3(const std::string& _name, const glm::vec3& _value) = 0;
		virtual void SetFloat4(const std::string& _name, const glm::vec4& _value) = 0;
		virtual void SetMat4(const std::string& _name, const glm::mat4& _value) = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& _filepath);
		static Ref<Shader> Create(const std::string& _name, const std::string& _vertexSource, const std::string& _fragmentSource);
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& _name, const Ref<Shader>& _shader);
		void Add(const Ref<Shader>& _shader);
		Ref<Shader> Load(const std::string& _filepath); //gives default name from file
		Ref<Shader> Load(const std::string& _name, const std::string& _filepath); //user can specify name using this overload

		Ref<Shader> Get(const std::string& _name);

		bool Exists(const std::string& _name) const;

	private:
		std::unordered_map<std::string, Ref<Shader>> mShaders;
	};

}