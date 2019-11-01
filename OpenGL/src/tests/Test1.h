#pragma once

#include <memory>

#include "Test.h"

#include "../VertexArray.h"
#include "../IndexBuffer.h"
#include "../Shader.h"

namespace test
{
	class Test1 : public test::Test
	{
	private:
		std::unique_ptr<VertexArray>	m_VAO;
		std::unique_ptr<VertexBuffer>	m_VBO;
		std::unique_ptr<IndexBuffer>	m_IBO;
		std::unique_ptr<Shader>			m_Shader;
	public:
		Test1();
		~Test1();

		void OnUpdate(float time, float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
