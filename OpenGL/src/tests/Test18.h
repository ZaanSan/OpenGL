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
	class Test18 : public test::Test
	{
	private:
		std::unique_ptr<Model>			m_Rock;
		std::unique_ptr<Model>			m_Planet;
		std::unique_ptr<VertexBuffer>	m_VBOInstanced;
		std::unique_ptr<Shader>			m_PlanetShader;
		std::unique_ptr<Shader>			m_RockShader;
		std::unique_ptr<Camera>			m_Camera;
		float							m_Time;
		glm::vec3						m_ClearColor;

	public:
		Test18();
		~Test18();

		void OnUpdate(float time, float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		static const int amount = 5000;
		glm::mat4 modelMatrices[amount];

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