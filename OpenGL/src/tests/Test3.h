#pragma once

#include <memory>

#include "Test.h"

#include "../VertexArray.h"
#include "../IndexBuffer.h"
#include "../Shader.h"
#include "../Texture.h"

namespace test
{
	class Test3 : public test::Test
	{
	private:
		std::unique_ptr<VertexArray>	m_VAO;
		std::unique_ptr<VertexBuffer>	m_VBO;
		std::unique_ptr<IndexBuffer>	m_IBO;
		std::unique_ptr<Shader>			m_Shader;
		std::unique_ptr<Texture>		m_Texture;
		std::unique_ptr<Texture>		m_Texture2;
		float							m_MixVal;
		glm::vec3						m_Translation;
		float							m_Angle;
		float							m_Scale;
	public:
		Test3();
		~Test3();

		void OnUpdate(float time, float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
