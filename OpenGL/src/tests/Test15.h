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
#include "../FrameBuffer.h"

namespace test
{
	class Test15 : public test::Test
	{
		enum class PostProcessingEffet : int
		{
			NONE,
			INVERSION,
			GREYSCALE,
			GREYSCALE_PB,
			K_SHARPEN,
			K_BLUR,
			K_EDGE
		};

	private:
		// off-screen rendering
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
		std::unique_ptr<FrameBuffer>	m_FrameBuffer;
		// quad rendering (post processing)
		std::unique_ptr<VertexArray>	m_VAOQuad;
		std::unique_ptr<VertexBuffer>	m_VBOQuad;
		std::unique_ptr<IndexBuffer>	m_IBOQuad;
		// Different Post processing chaserd
		std::unique_ptr<Shader>			m_ShaderQuad;
		std::unique_ptr<Shader>			m_InversionQuad;
		std::unique_ptr<Shader>			m_GreyscaleQuad;
		std::unique_ptr<Shader>			m_GreyscalePBQuad;
		std::unique_ptr<Shader>			m_KSharpenQuad;
		std::unique_ptr<Shader>			m_KBlurQuad;
		std::unique_ptr<Shader>			m_KEdgeQuad;
		PostProcessingEffet				m_Effect;

		float							m_Time;
		glm::vec3						m_ClearColor;

	public:
		Test15();
		~Test15();

		void OnUpdate(float time, float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		void bindRightShader();
		void drawRightShader();
		void unbindRindShader();
	};
}
#pragma once
