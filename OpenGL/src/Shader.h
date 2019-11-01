#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
	std::string GeometrySource;
};

class Shader
{
private:
	unsigned int							m_ShaderProgramID;
	std::unordered_map<std::string, int>	m_UniformLocationCache;

public:
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath);
	~Shader();

	void	Bind() const;
	void	Unbind() const;

	// Set uniforms
	void	SetUniform1i(const std::string&name, int value);
	void	SetUniform1f(const std::string&name, float value);
	void	SetUniform2f(const std::string&name, float v0, float v1);
	void	SetUniform2f(const std::string&name, const glm::vec2& vec);
	void	SetUniform3f(const std::string&name, float v0, float v1, float v2);
	void	SetUniform3f(const std::string&name, const glm::vec3& vec);
	void	SetUniform4f(const std::string&name, float v0, float v1, float v2, float v3);
	void	SetUniformMat4f(const std::string&name, const glm::mat4& matrix);

private:
	unsigned int		CreateShader(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader);
	ShaderProgramSource ParseShader(const char* vertexPath, const char* geometryPath, const char* fragmentPath);
	std::string			GetShaderSource(const char* shaderPath);
	unsigned int		CompileShader(unsigned int type, const std::string& source);
	int					GetUniformLocation(const std::string& name);
};

