#pragma once

#include <glm/vec3.hpp>

struct DirLight
{
	bool active;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void SetShaderValue(Shader& shader)
	{
		shader.SetUniform3f("dirLight.direction", direction);
		shader.SetUniform3f("dirLight.ambient", ambient);
		shader.SetUniform3f("dirLight.diffuse", diffuse);
		shader.SetUniform3f("dirLight.specular", specular);
	}
};

struct PointLight
{
	bool active;
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void SetShaderValue(Shader& shader, int lightNb)
	{
		shader.SetUniform3f("pointLights[" + std::to_string(lightNb) + "].position", position);
		shader.SetUniform3f("pointLights[" + std::to_string(lightNb) + "].ambient", ambient);
		shader.SetUniform3f("pointLights[" + std::to_string(lightNb) + "].diffuse", diffuse);
		shader.SetUniform3f("pointLights[" + std::to_string(lightNb) + "].specular", specular);
		shader.SetUniform1f("pointLights[" + std::to_string(lightNb) + "].constant", constant);
		shader.SetUniform1f("pointLights[" + std::to_string(lightNb) + "].linear", linear);
		shader.SetUniform1f("pointLights[" + std::to_string(lightNb) + "].quadratic", quadratic);
	}
};

struct SpotLight
{
	bool active;
	glm::vec3 position;
	glm::vec3 direction;
	float cutOff;
	float outerCutOff;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void SetShaderValue(Shader& shader)
	{
		shader.SetUniform3f("spotLight.position", position);
		shader.SetUniform3f("spotLight.direction", direction);
		shader.SetUniform3f("spotLight.ambient", ambient);
		shader.SetUniform3f("spotLight.diffuse", diffuse);
		shader.SetUniform3f("spotLight.specular", specular);
		shader.SetUniform1f("spotLight.cutOff", cutOff);
		shader.SetUniform1f("spotLight.outerCutOff", outerCutOff);
	}
};
