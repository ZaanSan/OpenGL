#pragma once

#include <memory>

#ifndef GLM_ENABLE_EXPERIMENTAL
# define GLM_ENABLE_EXPERIMENTAL
#endif
#include "glm/gtx/norm.hpp"

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
	class Test14 : public test::Test
	{
	private:
		std::unique_ptr<VertexArray>	m_VAO;
		std::unique_ptr<VertexBuffer>	m_VBO;
		std::unique_ptr<IndexBuffer>	m_IBO;
		std::unique_ptr<VertexArray>	m_VAOPlane;
		std::unique_ptr<VertexBuffer>	m_VBOPlane;
		std::unique_ptr<IndexBuffer>	m_IBOPlane;
		std::unique_ptr<Texture>		m_CubeTexture;
		std::unique_ptr<Texture>		m_PlaneTexture;
		std::unique_ptr<Shader>			m_Shader;
		std::unique_ptr<Camera>			m_Camera;
		float							m_Time;
		glm::vec3						m_ClearColor;

	public:
		Test14();
		~Test14();

		void OnUpdate(float time, float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
#pragma once
