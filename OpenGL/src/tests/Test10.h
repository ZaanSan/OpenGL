#pragma once

#include <memory>

#include "Test.h"

#include "../VertexArray.h"
#include "../IndexBuffer.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../Camera.h"
#include "../Light.h"
#include "../Model.h"

namespace test
{
	class Test10 : public test::Test
	{
	private:
		std::unique_ptr<VertexArray>	m_VAO;
		std::unique_ptr<VertexBuffer>	m_VBO;
		std::unique_ptr<IndexBuffer>	m_IBO;
		std::unique_ptr<Shader>			m_Shader;
		std::unique_ptr<Shader>			m_LightShader;
		std::unique_ptr<Camera>			m_Camera;
		std::unique_ptr<Model>			m_NanosuitModel;
		float							m_Time;
		glm::vec3						m_ClearColor;

	public:
		Test10();
		~Test10();

		void OnUpdate(float time, float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:

		PointLight m_PointLight[2] = {
			{
				true,
				 glm::vec3(2.0f,  0.2f,  2.0f),
				1.0f,
				0.09f,
				0.032f,
				glm::vec3(0.05f, 0.05f, 0.05f),
				glm::vec3(0.8f, 0.8f, 0.8f),
				glm::vec3(1.0f, 1.0f, 1.0f)
			},
			{
				true,
				glm::vec3(-2.0f, 0.4f, -2.0f),
				1.0f,
				0.09f,
				0.032f,
				glm::vec3(0.05f, 0.05f, 0.05f),
				glm::vec3(0.8f, 0.8f, 0.8f),
				glm::vec3(1.0f, 1.0f, 1.0f)
			}
		};
	};
}
