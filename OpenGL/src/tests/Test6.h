#pragma once

#include <memory>

#include "Test.h"

#include "../VertexArray.h"
#include "../IndexBuffer.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../Camera.h"

namespace test
{
	class Test6 : public test::Test
	{
	private:
		std::unique_ptr<VertexArray>	m_VAO;
		std::unique_ptr<VertexBuffer>	m_VBO;
		std::unique_ptr<IndexBuffer>	m_IBO;
		std::unique_ptr<Shader>			m_Shader;
		std::unique_ptr<Shader>			m_LightShader;
		std::unique_ptr<Camera>			m_Camera;
		float							m_Time;
		float							m_AmbiantStrength;
		float							m_SpecularStrength;
		int								m_SpecularShininess;

	public:
		Test6();
		~Test6();

		void OnUpdate(float time, float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		glm::vec3 m_CubePositions[1] = {
				glm::vec3(0.0f,  0.0f,  0.0f)
		};

		glm::vec3 m_LightPositions[1] = {
				glm::vec3(1.2f,  1.0f,  2.0f)
		};
	};
}
