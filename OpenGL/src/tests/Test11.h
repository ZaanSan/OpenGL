#pragma once
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
	class Test11 : public test::Test
	{
	private:
		std::unique_ptr<VertexArray>	m_VAO;
		std::unique_ptr<VertexBuffer>	m_VBO;
		std::unique_ptr<IndexBuffer>	m_IBO;
		std::unique_ptr<Shader>			m_Shader;
		std::unique_ptr<Shader>			m_LightShader;
		std::unique_ptr<Camera>			m_Camera;
		std::unique_ptr<Model>			m_Sponza;
		float							m_Time;
		glm::vec3						m_ClearColor;

	public:
		Test11();
		~Test11();

		void OnUpdate(float time, float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
