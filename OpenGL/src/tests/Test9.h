#pragma once

#include <memory>

#include "Test.h"

#include "../VertexArray.h"
#include "../IndexBuffer.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../Camera.h"
#include "../Light.h"

namespace test
{
	class Test9 : public test::Test
	{
	private:
		std::unique_ptr<VertexArray>	m_VAO;
		std::unique_ptr<VertexBuffer>	m_VBO;
		std::unique_ptr<IndexBuffer>	m_IBO;
		std::unique_ptr<Shader>			m_Shader;
		std::unique_ptr<Shader>			m_LightShader;
		std::unique_ptr<Camera>			m_Camera;
		std::unique_ptr<Texture>		m_DiffuseMap;
		std::unique_ptr<Texture>		m_SpecularMap;
		float							m_Time;
		glm::vec3						m_ClearColor;

	public:
		Test9();
		~Test9();

		void OnUpdate(float time, float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		glm::vec3 m_CubePositions[10] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		DirLight m_DirLight = {
			true,
			glm::vec3(-0.2f, -1.0f, -0.3f),
			glm::vec3(0.05f, 0.05f, 0.05f),
			glm::vec3(0.4f, 0.4f, 0.4f),
			glm::vec3(0.5f, 0.5f, 0.5f)
		};

		PointLight m_PointLight[4] = {
			{
				true,
				 glm::vec3(0.7f,  0.2f,  2.0f),
				1.0f,
				0.09f,
				0.032f,
				glm::vec3(0.05f, 0.05f, 0.05f),
				glm::vec3(0.8f, 0.8f, 0.8f),
				glm::vec3(1.0f, 1.0f, 1.0f)
			},
			{
				true,
				glm::vec3(2.3f, -3.3f, -4.0f),
				1.0f,
				0.09f,
				0.032f,
				glm::vec3(0.05f, 0.05f, 0.05f),
				glm::vec3(0.8f, 0.8f, 0.8f),
				glm::vec3(1.0f, 1.0f, 1.0f)
			},
			{
				true,
				glm::vec3(-4.0f,  2.0f, -12.0f),
				1.0f,
				0.09f,
				0.032f,
				glm::vec3(0.05f, 0.05f, 0.05f),
				glm::vec3(0.8f, 0.8f, 0.8f),
				glm::vec3(1.0f, 1.0f, 1.0f)
			},
			{
				true,
				glm::vec3(0.0f,  0.0f, -3.0f),
				1.0f,
				0.09f,
				0.032f,
				glm::vec3(0.05f, 0.05f, 0.05f),
				glm::vec3(0.8f, 0.8f, 0.8f),
				glm::vec3(1.0f, 1.0f, 1.0f)
			}
		};

		SpotLight m_SpotLight = {
			true,
			glm::vec3(),
			glm::vec3(),
			glm::cos(glm::radians(12.5f)),
			glm::cos(glm::radians(17.5f)),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),

		};
	};
}
