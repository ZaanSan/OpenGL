#include "Test17.h"

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

	Test17::Test17()
		: m_Time(0.0f),
		m_ClearColor(glm::vec3(0.0f))
	{
		m_Nanosuit = std::make_unique<Model>("res/models/nanosuit/nanosuit.obj");

		m_ExplodeShader = std::make_unique<Shader>("res/shaders/Test17/vertex.glsl", "res/shaders/Test17/geometry.glsl", "res/shaders/Test17/fragment.glsl");
		m_NormalShader = std::make_unique<Shader>("res/shaders/Test17/vertex_normal.glsl", "res/shaders/Test17/geometry_normal.glsl", "res/shaders/Test17/fragment_normal.glsl");
		m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
	}

	Test17::~Test17() = default;

	void Test17::OnUpdate(float time, float deltaTime)
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

	void Test17::OnRender()
	{
		Renderer renderer;

		glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, 1.0f);

		glm::mat4 projection = glm::perspective(glm::radians(m_Camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = m_Camera->GetViewMatrix();

		m_ExplodeShader->Bind();
		m_ExplodeShader->SetUniformMat4f("projection", projection);
		m_ExplodeShader->SetUniformMat4f("view", view);
		m_ExplodeShader->SetUniform1f("time", m_Time);
		m_ExplodeShader->SetUniform1f("material.shininess", 32);
		m_DirLight.SetShaderValue(*m_ExplodeShader);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		m_ExplodeShader->SetUniformMat4f("model", model);
		renderer.Draw(*m_Nanosuit, *m_ExplodeShader);
		m_ExplodeShader->Unbind();

		m_NormalShader->Bind();
		m_NormalShader->SetUniformMat4f("projection", projection);
		m_NormalShader->SetUniformMat4f("view", view);
		m_NormalShader->SetUniformMat4f("model", model);
		renderer.Draw(*m_Nanosuit, *m_NormalShader);
		m_NormalShader->Unbind();
	}

	void Test17::OnImGuiRender()
	{
		static int effect_idx = 0;

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
