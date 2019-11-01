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
	class Test17 : public test::Test
	{
	private:
		std::unique_ptr<Model>			m_Nanosuit;
		std::unique_ptr<Shader>			m_ExplodeShader;
		std::unique_ptr<Shader>			m_NormalShader;
		std::unique_ptr<Camera>			m_Camera;

		float							m_Time;
		glm::vec3						m_ClearColor;

	public:
		Test17();
		~Test17();

		void OnUpdate(float time, float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		DirLight m_DirLight =
		{
			true,
			glm::vec3(0.0f, -0.5f, -0.5f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f)
		};
	};
}