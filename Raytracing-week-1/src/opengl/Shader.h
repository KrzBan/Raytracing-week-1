#pragma once

#include <iostream>
#include <unordered_map>

#include "Utility.h"

class Shader
{
public:
	Shader(const std::string& name, const char* vertexPath, const char* fragmentPath);

	const std::string& GetName() const { return m_Name; }

	void Bind() const;

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetIntArray(const std::string& name, int* values, uint32_t count) const;
	void SetFloat(const std::string& name, float value) const;
	void SetFloat4(const std::string& name, const glm::vec4&) const;
	void SetMat4f(const std::string& name,const glm::mat4& mat)const;
private:
	uint32_t ID;
	const std::string m_Name;
};

class ShaderLibrary {
public:
	void Add(const std::shared_ptr<Shader>& shader);

	std::shared_ptr<Shader> Load(const std::string& name, const char* vertexPath, const char* fragmentPath);

	std::shared_ptr<Shader> Get(const std::string& name)const;

	bool Exists(const std::string& name) const;
private:
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
};
