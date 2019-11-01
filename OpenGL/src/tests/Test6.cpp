#include "Test6.h"

#include "imgui/imgui.h"

#include "../VertexBufferLayout.h"
#include "../InputSystem.h"

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

namespace test
{

	Test6::Test6()
		: m_Time(0.0f),
		m_AmbiantStrength(0.1f),
		m_SpecularStrength(0.5f),
		m_SpecularShininess(32)
	{
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		};

		unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3,
			4, 5, 7,
			5, 6, 7,
			8, 9, 11,
			9, 10, 11,
			12, 13, 15,
			13, 14, 15,
			16, 17, 19,
			17, 18, 19,
			20, 21, 23,
			21, 22, 23
		};

		m_VAO = std::make_unique<VertexArray>();
		m_VAO->Bind();
		
		m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		m_IBO = std::make_unique<IndexBuffer>(indices, 36);
		m_VAO->AddBuffer(*m_VBO, layout);
		m_Shader = std::make_unique<Shader>("res/shaders/Test6/vertex.glsl", "res/shaders/Test6/fragment.glsl");
		m_Shader->SetUniform3f("objectColor", 1.0f, 0.5f, 0.31f);
		m_Shader->SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
		m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
		m_Shader->Unbind();

		m_LightShader = std::make_unique<Shader>("res/shaders/Test6/vertex.glsl", "res/shaders/Test6/lightFragment.glsl");
		m_LightShader->Unbind();

		m_VAO->Unbind();
	}

	Test6::~Test6() = default;

	void Test6::OnUpdate(float time, float deltaTime)
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

	void Test6::OnRender()
	{
		Renderer renderer;
		glm::mat4 projection = glm::perspective(glm::radians(m_Camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = m_Camera->GetViewMatrix();

		m_VAO->Bind();

		// Render cube
		m_Shader->Bind();
		m_Shader->SetUniform3f("lightPos", m_LightPositions[0]);
		m_Shader->SetUniform1f("ambientStrength", m_AmbiantStrength);
		m_Shader->SetUniform1f("specularStrength", m_SpecularStrength);
		m_Shader->SetUniform1i("shininess", m_SpecularShininess);
		m_Shader->SetUniformMat4f("view", m_Camera->GetViewMatrix());
		m_Shader->SetUniformMat4f("projection", projection);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, m_CubePositions[0]);
		m_Shader->SetUniformMat4f("model", model);
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
		m_Shader->Unbind();

		// Render light
		m_LightShader->Bind();
		m_LightShader->SetUniformMat4f("view", m_Camera->GetViewMatrix());
		m_LightShader->SetUniformMat4f("projection", projection);
		model = glm::mat4(1.0f);
		model = glm::translate(model, m_LightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f));
		m_LightShader->SetUniformMat4f("model", model);
		renderer.Draw(*m_VAO, *m_IBO, *m_LightShader);
		m_LightShader->Unbind();
	}

	void Test6::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderFloat3("Light position", &m_LightPositions[0][0], -3.0f, 3.0f);
		ImGui::SliderFloat("Ambient strength", &m_AmbiantStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular strength", &m_SpecularStrength, 0.0f, 1.0f);
		ImGui::SliderInt("Specular shininess", &m_SpecularShininess, 2, 256);
	}

}
