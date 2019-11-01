#include "Test16.h"

#include "imgui/imgui.h"

#include "../VertexBufferLayout.h"
#include "../InputSystem.h"
#include "../TextureManager.h"

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

/**
 * This example doesn't support window resizing because it uses hard coded
 *  constants SRC_WIDTH and SRC_HEIGHT when setting up the FrameBuffer
 */

namespace test
{

	Test16::Test16()
		: m_Time(0.0f),
		m_ClearColor(glm::vec3(0.0f))
	{
		float skyboxVertices[] = {
			// positions   
		    -1.0f,  1.0f, -1.0f,
		    -1.0f, -1.0f, -1.0f,
		     1.0f, -1.0f, -1.0f,
		     1.0f,  1.0f, -1.0f,

		    -1.0f, -1.0f,  1.0f,
		    -1.0f, -1.0f, -1.0f,
		    -1.0f,  1.0f, -1.0f,
		    -1.0f,  1.0f,  1.0f,

		     1.0f, -1.0f, -1.0f,
		     1.0f, -1.0f,  1.0f,
		     1.0f,  1.0f,  1.0f,
		     1.0f,  1.0f, -1.0f,

		    -1.0f, -1.0f,  1.0f,
		    -1.0f,  1.0f,  1.0f,
		     1.0f,  1.0f,  1.0f,
		     1.0f, -1.0f,  1.0f,

		    -1.0f,  1.0f, -1.0f,
		     1.0f,  1.0f, -1.0f,
		     1.0f,  1.0f,  1.0f,
		    -1.0f,  1.0f,  1.0f,

		    -1.0f, -1.0f, -1.0f,
		    -1.0f, -1.0f,  1.0f,
		     1.0f, -1.0f, -1.0f,
		     1.0f, -1.0f,  1.0f,
		};

		unsigned int skyboxIndices[] = {
			0, 1, 2,
			2, 3, 0,
			4, 5, 6,
			6, 7, 4,
			8, 9, 10,
			10, 11, 8,
			12, 13, 14,
			14, 15, 12,
			16, 17, 18,
			18, 19, 16,
			20, 21, 22,
			22, 21, 23
		};

		std::vector<std::string> skyboxFaces = 
		{
			"res/skybox/right.jpg",
			"res/skybox/left.jpg",
			"res/skybox/top.jpg",
			"res/skybox/bottom.jpg",
			"res/skybox/front.jpg",
			"res/skybox/back.jpg"
		};
		

		// Nanosuit model
		m_Nanosuit = std::make_unique<Model>("res/models/nanosuit_reflection/nanosuit.obj");


		// Skybox VAO
		m_VAOSky = std::make_unique<VertexArray>();
		m_VAOSky->Bind();
		m_VBOSky = std::make_unique<VertexBuffer>(skyboxVertices, sizeof(skyboxVertices));
		// Same indices as Cube
		m_IBOSky = std::make_unique<IndexBuffer>(skyboxIndices, 36);
		VertexBufferLayout layoutSkybox;
		layoutSkybox.Push<float>(3);
		m_VAOSky->AddBuffer(*m_VBOSky, layoutSkybox);
		m_VAOSky->Unbind();
		m_IBOSky->Unbind();

		m_SkyCubemap = std::make_unique<Cubemap>(skyboxFaces);

		// Chrome suit
		m_ShaderChrome = std::make_unique<Shader>("res/shaders/Test16/vertex.glsl", "res/shaders/Test16/fragment_chrome.glsl");
		m_ShaderChrome->Unbind();
		// Glass suit
		m_ShaderGlass = std::make_unique<Shader>("res/shaders/Test16/vertex.glsl", "res/shaders/Test16/fragment_glass.glsl");
		m_ShaderGlass->Unbind();
		// Reflection suit
		m_ShaderReflection = std::make_unique<Shader>("res/shaders/Test16/vertex.glsl", "res/shaders/Test16/fragment_reflection_model.glsl");
		m_ShaderReflection->Unbind();

		// Define default shader to chrome
		m_Shader = m_ShaderChrome;

		m_ShaderSkybox = std::make_unique<Shader>("res/shaders/Test16/vertex_skybox.glsl", "res/shaders/Test16/fragment_skybox.glsl");
		m_ShaderSkybox->Unbind();

		m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
	}

	Test16::~Test16() = default;

	void Test16::OnUpdate(float time, float deltaTime)
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

	void Test16::OnRender()
	{
		Renderer renderer;

		glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, 1.0f);

		glm::mat4 projection = glm::perspective(glm::radians(m_Camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = m_Camera->GetViewMatrix();

		// Bind sky cubemap for environment mapping
		m_SkyCubemap->Bind(4);

		// Set shader values
		m_Shader->Bind();
		m_Shader->SetUniform1i("skybox", 4);
		m_Shader->SetUniformMat4f("view", view);
		m_Shader->SetUniformMat4f("projection", projection);
		m_Shader->SetUniform3f("cameraPos", m_Camera->Position);
		m_Shader->SetUniform1f("material.shininess", 32.0f);
		m_DirLight.SetShaderValue(*m_Shader);


		// Draw cube : Backface culling
		renderer.EnableBackfaceCulling();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		m_Shader->SetUniformMat4f("model", model);
		renderer.Draw(*m_Nanosuit, *m_Shader);

		// Without translation
		m_SkyCubemap->Unbind();
		glm::mat4 viewSkybox = glm::mat4(glm::mat3(m_Camera->GetViewMatrix()));
		m_SkyCubemap->Bind(0);

		// Draw skybox
		renderer.SetDepthFunction(Renderer::DepthFunction::LEQUAL);
		m_ShaderSkybox->Bind();
		m_ShaderSkybox->SetUniformMat4f("view", viewSkybox);
		m_ShaderSkybox->SetUniformMat4f("projection", projection);
		m_VAOSky->Bind();
		renderer.Draw(*m_VAOSky, *m_IBOSky, *m_ShaderSkybox);
		m_SkyCubemap->Unbind();
		m_VAOSky->Unbind();
		m_ShaderSkybox->Unbind();
		renderer.SetDepthFunction(Renderer::DepthFunction::LESS);
	}

	void Test16::OnImGuiRender()
	{
		static int effect_idx = 0;

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		if (ImGui::Combo("Shader type", &effect_idx, "Chrome (Reflection)\0Glass (Refraction)\0Model reflection\0"))
		{
			switch (effect_idx)
			{
			case 0: m_Shader = m_ShaderChrome; break;
			case 1: m_Shader = m_ShaderGlass; break;
			case 2: m_Shader = m_ShaderReflection; break;
			}
		}
	}
}
