#include "Shader.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Renderer.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
	: Shader(vertexPath, nullptr, fragmentPath)
{
}

Shader::Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
{
	ShaderProgramSource source = ParseShader(vertexPath, geometryPath, fragmentPath);
	m_ShaderProgramID = CreateShader(source.VertexSource, source.GeometrySource, source.FragmentSource);
	GLCall(glUseProgram(m_ShaderProgramID));
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_ShaderProgramID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_ShaderProgramID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
	GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniform2f(const std::string& name, const glm::vec2& vec)
{
	GLCall(glUniform2f(GetUniformLocation(name), vec.x, vec.y));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& vec)
{
	GLCall(glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

unsigned Shader::CreateShader(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader)
{
	int			 result;
	unsigned int gs = 0;
	const bool hasGShader = !geometryShader.empty();
	
	GLCall(unsigned int program = glCreateProgram());

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	GLCall(glAttachShader(program, vs));
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	GLCall(glAttachShader(program, fs));
	if (hasGShader)
	{
		gs = CompileShader(GL_GEOMETRY_SHADER, geometryShader);
		GLCall(glAttachShader(program, gs));
	}

	GLCall(glLinkProgram(program));
	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
		auto message = static_cast<char *>(alloca(length * sizeof(char)));
		GLCall(glGetProgramInfoLog(program, length, &length, message));
		std::cout << "Failed to link shader program" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteProgram(program));
		return 0;
	}
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
	if (hasGShader)
		GLCall(glDeleteShader(gs));

	return program;
}

ShaderProgramSource Shader::ParseShader(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
{
	ShaderProgramSource source;
	source.VertexSource = GetShaderSource(vertexPath);
	source.GeometrySource = (geometryPath != nullptr) ? GetShaderSource(geometryPath) : "";
	source.FragmentSource = GetShaderSource(fragmentPath);
	return source;
}

std::string Shader::GetShaderSource(const char* shaderPath)
{
	std::ifstream shaderFile;
	std::stringstream shaderStream;

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(shaderPath);
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR : Can't read shader file : " << shaderPath << std::endl;
	}
	return shaderStream.str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	int				result;
	GLCall(unsigned int	id = glCreateShader(type));
	const char*		src = source.c_str();

	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = static_cast<char *>(alloca(length * sizeof(char)));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile ";
		switch (type)
		{
		case GL_VERTEX_SHADER: std::cout << "vertex"; break;
		case GL_FRAGMENT_SHADER: std::cout << "fragment"; break;
		case GL_GEOMETRY_SHADER: std::cout << "geometry"; break;
		}
		std::cout << " shader" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	GLCall(int location = glGetUniformLocation(m_ShaderProgramID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exists" << std::endl;
	m_UniformLocationCache[name] = location;
	return location;
}
