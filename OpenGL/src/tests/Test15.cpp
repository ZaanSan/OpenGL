#include "Test15.h"

#include <algorithm>

#include "imgui/imgui.h"

#include "../VertexBufferLayout.h"
#include "../InputSystem.h"
#include "../TextureManager.h"
#include <iostream>

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

/**
 * This example doesn't support window resizing because it uses hard coded 
 *  constants SRC_WIDTH and SRC_HEIGHT when setting up the FrameBuffer
 */

namespace test
{

	Test15::Test15()
		: m_Effect(PostProcessingEffet::NONE),
		m_Time(0.0f),
		m_ClearColor(glm::vec3(0.0f))
	{

		float vertices[] = {
			// Back face
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
			// Front face
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
			// Left face
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
			// Right face
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
			// Bottom face
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
			// Top face
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left  
		};

		unsigned int indices[] = {
			0, 1, 2,
			1, 0, 3,
			4, 5, 6,
			6, 7, 4,
			8, 9, 10,
			10, 11, 8,
			12, 13, 14,
			13, 12, 15,
			16, 17, 18,
			18, 19, 16,
			20, 21, 22,
			21, 20, 23
		};

		float planeVertices[] = {
			// positions          // texture Coords (-0.51f as y coordinate to prevent z-fighting)
			 5.0f, -0.51f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.51f,  5.0f,  0.0f, 0.0f,
			-5.0f, -0.51f, -5.0f,  0.0f, 2.0f,
			 5.0f, -0.51f, -5.0f,  2.0f, 2.0f
		};

		unsigned int planeIndices[] = {
			0, 1, 3,
			1, 2, 3
		};

		float windowVertices[] = {
			// positions         // texture coords
			0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
			0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
			1.0f,  0.5f,  0.0f,  1.0f,  1.0f,
			1.0f, -0.5f,  0.0f,  1.0f,  0.0f
		};

		float quadVertices[] = {
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		// Cube VAO
		m_VAO = std::make_unique<VertexArray>();
		m_VAO->Bind();
		m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_IBO = std::make_unique<IndexBuffer>(indices, 36);
		m_VAO->AddBuffer(*m_VBO, layout);
		m_VAO->Unbind();
		m_IBO->Unbind();

		// Plane VAO
		m_VAOPlane = std::make_unique<VertexArray>();
		m_VAOPlane->Bind();
		m_VBOPlane = std::make_unique<VertexBuffer>(planeVertices, sizeof(planeVertices));
		m_IBOPlane = std::make_unique<IndexBuffer>(planeIndices, 6);
		// Same data layout as Cube
		m_VAOPlane->AddBuffer(*m_VBOPlane, layout);
		m_VAOPlane->Unbind();
		m_IBOPlane->Unbind();

		// Off-screen rendering shader
		m_Shader = std::make_unique<Shader>("res/shaders/Test15/vertex.glsl", "res/shaders/Test15/fragment.glsl");
		m_Shader->Unbind();

		// Quad VAO (Post processing)
		m_VAOQuad = std::make_unique<VertexArray>();
		m_VAOQuad->Bind();
		m_VBOQuad = std::make_unique<VertexBuffer>(quadVertices, sizeof(quadVertices));
		VertexBufferLayout layoutQuad;
		layoutQuad.Push<float>(2);
		layoutQuad.Push<float>(2);
		m_IBOQuad = std::make_unique<IndexBuffer>(planeIndices, 6);
		m_VAOQuad->AddBuffer(*m_VBOQuad, layoutQuad);
		m_VAOQuad->Unbind();
		m_IBOQuad->Unbind();

		// Post-processing shaders
		m_ShaderQuad = std::make_unique<Shader>("res/shaders/Test15/simple_v.glsl", "res/shaders/Test15/simple_f.glsl");
		m_ShaderQuad->Unbind();
		m_InversionQuad = std::make_unique<Shader>("res/shaders/Test15/simple_v.glsl", "res/shaders/Test15/inversion_f.glsl");
		m_InversionQuad->Unbind();
		m_GreyscaleQuad = std::make_unique<Shader>("res/shaders/Test15/simple_v.glsl", "res/shaders/Test15/greyscale_f.glsl");
		m_GreyscaleQuad->Unbind();
		m_GreyscalePBQuad = std::make_unique<Shader>("res/shaders/Test15/simple_v.glsl", "res/shaders/Test15/greyscale_pb_f.glsl");
		m_GreyscalePBQuad->Unbind();
		m_KSharpenQuad = std::make_unique<Shader>("res/shaders/Test15/simple_v.glsl", "res/shaders/Test15/kernel_sharpen_f.glsl");
		m_KSharpenQuad->Unbind();
		m_KBlurQuad = std::make_unique<Shader>("res/shaders/Test15/simple_v.glsl", "res/shaders/Test15/kernel_blur_f.glsl");
		m_KBlurQuad->Unbind();
		m_KEdgeQuad = std::make_unique<Shader>("res/shaders/Test15/simple_v.glsl", "res/shaders/Test15/kernel_edge-detection_f.glsl");
		m_KEdgeQuad->Unbind();

		// Off-screen rendering frame buffer
		m_FrameBuffer = std::make_unique<FrameBuffer>(Texture::Type::POST_PROCESSING, SCR_WIDTH, SCR_HEIGHT, RenderBuffer::Type::DEPTH24_STENCIL8);

		m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

		m_CubeTexture = std::make_unique<Texture>("res/textures/container.jpg");
		m_PlaneTexture = std::make_unique<Texture>("res/textures/metal.jpg");
	}

	Test15::~Test15() = default;

	void Test15::OnUpdate(float time, float deltaTime)
	{
		InputSystem& is = InputSystem::Instance();

		m_Time = time;
		if (is.GetKeyState(GLFW_KEY_W) != GLFW_RELEASE)
			m_Camera->ProcessMovement(CameraMovement::FORWARD, deltaTime);
		if (is.GetKeyState(GLFW_KEY_S) != GLFW_RELEASE)
			m_Camera->ProcessMovement(CameraMovement::BACKWARD, deltaTime);
		if (is.GetKeyState(GLFW_KEY_A) != GLFW_RELEASE)
			m_Camera->ProcessMovement(CameraMovement::LEFT, deltaTime);
		if (is.GetKeyState(GLFW_KEY_D) != GLFW_RELEASE)
			m_Camera->ProcessMovement(CameraMovement::RIGHT, deltaTime);
		if (is.GetKeyState(GLFW_KEY_UP) != GLFW_RELEASE)
			m_Camera->ProcessMovement(CameraMovement::PITCH_UP, deltaTime);
		if (is.GetKeyState(GLFW_KEY_DOWN) != GLFW_RELEASE)
			m_Camera->ProcessMovement(CameraMovement::PITCH_DOWN, deltaTime);
		if (is.GetKeyState(GLFW_KEY_RIGHT) != GLFW_RELEASE)
			m_Camera->ProcessMovement(CameraMovement::YAW_RIGHT, deltaTime);
		if (is.GetKeyState(GLFW_KEY_LEFT) != GLFW_RELEASE)
			m_Camera->ProcessMovement(CameraMovement::YAW_LEFT, deltaTime);
		m_Camera->ProcessZoom(is.GetScrollOffset().y);
	}

	void Test15::OnRender()
	{
		Renderer renderer;

		glm::mat4 projection = glm::perspective(glm::radians(m_Camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = m_Camera->GetViewMatrix();

		// Off-screen rendering
		m_FrameBuffer->Bind();
		renderer.EnableDepthTesting();

		// make sure we clear the framebuffer's content
		glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, 1.0f);
		renderer.Clear();

		// Set shader values
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("view", view);
		m_Shader->SetUniformMat4f("projection", projection);

		// Draw 2 cubes : Backface culling
		renderer.EnableBackfaceCulling();
		m_VAO->Bind();
		m_CubeTexture->Bind(0);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		m_Shader->SetUniformMat4f("model", model);
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		m_Shader->SetUniformMat4f("model", model);
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
		m_CubeTexture->Unbind();
		m_VAO->Unbind();
		renderer.DisableBackfaceCulling();

		// Draw plane
		m_VAOPlane->Bind();
		m_PlaneTexture->Bind(0);
		model = glm::mat4(1.0f);
		m_Shader->SetUniformMat4f("model", model);
		renderer.Draw(*m_VAOPlane, *m_IBOPlane, *m_Shader);
		m_PlaneTexture->Unbind();
		m_VAOPlane->Unbind();

		// End of off-screen rendering
		m_Shader->Unbind();
		m_FrameBuffer->Unbind();
		renderer.DisableDepthTesting();
		
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		renderer.Clear();

		bindRightShader();
		m_VAOQuad->Bind();
		m_FrameBuffer->BindTextureBuffer();
		drawRightShader();
		m_FrameBuffer->UnbindTextureBuffer();
		m_VAOQuad->Unbind();
		unbindRindShader();
	}

	void Test15::OnImGuiRender()
	{
		static int effect_idx = 0;

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		if (ImGui::Combo("Post processing effect", &effect_idx, "None\0Inversion\0Greyscale\0Greyscale(PB)\0K-Sharpen\0K-Blur\0K-EdgeDetection\0"))
		{
			switch (effect_idx)
			{
				case 0: m_Effect = PostProcessingEffet::NONE; break;
				case 1: m_Effect = PostProcessingEffet::INVERSION; break;
				case 2: m_Effect = PostProcessingEffet::GREYSCALE; break;
				case 3: m_Effect = PostProcessingEffet::GREYSCALE_PB; break;
				case 4: m_Effect = PostProcessingEffet::K_SHARPEN; break;
				case 5: m_Effect = PostProcessingEffet::K_BLUR; break;
				case 6: m_Effect = PostProcessingEffet::K_EDGE; break;
			}
		}
		ImGui::ColorEdit3("ClearColor", &m_ClearColor[0]);
	}

	// Private methods

	void Test15::bindRightShader()
	{
		switch (m_Effect)
		{
		case PostProcessingEffet::NONE:
			m_ShaderQuad->Bind();
			m_ShaderQuad->SetUniform1i("screenTexture", 0);
			break;
		case PostProcessingEffet::INVERSION:
			m_InversionQuad->Bind();
			m_InversionQuad->SetUniform1i("screenTexture", 0);
			break;
		case PostProcessingEffet::GREYSCALE:
			m_GreyscaleQuad->Bind();
			m_GreyscaleQuad->SetUniform1i("screenTexture", 0);
			break;
		case PostProcessingEffet::GREYSCALE_PB:
			m_GreyscalePBQuad->Bind();
			m_GreyscalePBQuad->SetUniform1i("screenTexture", 0);
			break;
		case PostProcessingEffet::K_SHARPEN:
			m_KSharpenQuad->Bind();
			m_KSharpenQuad->SetUniform1i("screenTexture", 0);
			break;
		case PostProcessingEffet::K_BLUR:
			m_KBlurQuad->Bind();
			m_KBlurQuad->SetUniform1i("screenTexture", 0);
			break;
		case PostProcessingEffet::K_EDGE:
			m_KEdgeQuad->Bind();
			m_KEdgeQuad->SetUniform1i("screenTexture", 0);
			break;
		}
	}

	void Test15::drawRightShader()
	{
		Renderer renderer;

		switch (m_Effect)
		{
		case PostProcessingEffet::NONE: renderer.Draw(*m_VAOQuad, *m_IBOQuad, *m_ShaderQuad); break;
		case PostProcessingEffet::INVERSION: renderer.Draw(*m_VAOQuad, *m_IBOQuad, *m_InversionQuad); break;
		case PostProcessingEffet::GREYSCALE: renderer.Draw(*m_VAOQuad, *m_IBOQuad, *m_GreyscaleQuad); break;
		case PostProcessingEffet::GREYSCALE_PB: renderer.Draw(*m_VAOQuad, *m_IBOQuad, *m_GreyscalePBQuad); break;
		case PostProcessingEffet::K_SHARPEN: renderer.Draw(*m_VAOQuad, *m_IBOQuad, *m_KSharpenQuad); break;
		case PostProcessingEffet::K_BLUR: renderer.Draw(*m_VAOQuad, *m_IBOQuad, *m_KBlurQuad); break;
		case PostProcessingEffet::K_EDGE: renderer.Draw(*m_VAOQuad, *m_IBOQuad, *m_KEdgeQuad); break;
		}
	}


	void Test15::unbindRindShader()
	{
		switch (m_Effect)
		{
		case PostProcessingEffet::NONE: m_ShaderQuad->Unbind(); break;
		case PostProcessingEffet::INVERSION: m_InversionQuad->Unbind(); break;
		case PostProcessingEffet::GREYSCALE: m_GreyscaleQuad->Unbind(); break;
		case PostProcessingEffet::GREYSCALE_PB: m_GreyscalePBQuad->Unbind(); break;
		case PostProcessingEffet::K_SHARPEN: m_KSharpenQuad->Unbind(); break;
		case PostProcessingEffet::K_BLUR: m_KBlurQuad->Unbind(); break;
		case PostProcessingEffet::K_EDGE: m_KEdgeQuad->Unbind(); break;
		}
	}


}
