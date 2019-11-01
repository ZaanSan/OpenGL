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
#include "../FrameBuffer.h"
#include "../Shape.h"

namespace test
{
	class Test20 : public test::Test
	{
	private:
		std::unique_ptr<Shape>			m_Cube;
		std::unique_ptr<Shape>			m_Plane;
		std::unique_ptr<Shape>			m_DebugQuad;
		std::unique_ptr<FrameBuffer>	m_DepthMap;
		std::unique_ptr<Texture>		m_WoodTexture;
		std::unique_ptr<Shader>			m_DepthShader;
		std::unique_ptr<Shader>			m_DebugDepthShader;
		std::unique_ptr<Shader>			m_Shader;
		std::unique_ptr<Camera>			m_Camera;
		float							m_Time;
		glm::vec3						m_ClearColor;

	public:
		Test20();
		~Test20();

		void OnUpdate(float time, float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		void RenderScene(Shader& shader);
	};
}