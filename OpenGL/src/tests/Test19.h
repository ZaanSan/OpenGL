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
#include "../Camera.h"
#include "../Cubemap.h"
#include "../Model.h"
#include "../Light.h"

namespace test
{
	class Test19 : public test::Test
	{
	private:
		std::unique_ptr<VertexArray>	m_CubeVAO;
		std::unique_ptr<VertexBuffer>	m_CubeVBO;
		std::unique_ptr<IndexBuffer>	m_CubeIBO;
		std::unique_ptr<Shader>			m_Shader;
		std::unique_ptr<Camera>			m_Camera;
		float							m_Time;
		glm::vec3						m_ClearColor;

	public:
		Test19();
		~Test19();

		void OnUpdate(float time, float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}