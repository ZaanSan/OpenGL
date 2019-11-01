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
	class Test16 : public test::Test
	{
	private:
		// Models
		std::unique_ptr<Model>			m_Nanosuit;

		// Skybox
		std::unique_ptr<VertexArray>	m_VAOSky;
		std::unique_ptr<VertexBuffer>	m_VBOSky;
		std::unique_ptr<IndexBuffer>	m_IBOSky;
		std::unique_ptr<Cubemap>		m_SkyCubemap;
		// Shaders
		std::shared_ptr<Shader>			m_ShaderChrome;
		std::shared_ptr<Shader>			m_ShaderReflection;
		std::shared_ptr<Shader>			m_ShaderGlass;
		std::shared_ptr<Shader>			m_Shader;
		std::unique_ptr<Shader>			m_ShaderSkybox;
		// Camera
		std::unique_ptr<Camera>			m_Camera;

		float							m_Time;
		glm::vec3						m_ClearColor;

	public:
		Test16();
		~Test16();

		void OnUpdate(float time, float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:

		DirLight m_DirLight = {
			true,
			glm::vec3(-0.2f, -1.0f, -0.3f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(0.4f, 0.4f, 0.4f),
			glm::vec3(0.5f, 0.5f, 0.5f)
		};
	};
}