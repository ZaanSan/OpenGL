#include "Test10.h"

#include "imgui/imgui.h"

#include "../VertexBufferLayout.h"
#include "../InputSystem.h"

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

namespace test
{

	Test10::Test10()
		: m_Time(0.0f),
		m_ClearColor(glm::vec3(0.0f))
	{
		
		float vertices[] = {
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
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

		// Use for light
		m_VAO = std::make_unique<VertexArray>();
		m_VAO->Bind();
		m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_IBO = std::make_unique<IndexBuffer>(indices, 36);
		m_VAO->AddBuffer(*m_VBO, layout);
		m_LightShader = std::make_unique<Shader>("res/shaders/Test10/vertex.glsl", "res/shaders/Test10/lightFragment.glsl");
		m_LightShader->Unbind();
		m_VAO->Unbind();

		m_Shader = std::make_unique<Shader>("res/shaders/Test10/vertex.glsl", "res/shaders/Test10/fragment.glsl");
		m_Shader->Unbind();
		m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
		m_NanosuitModel = std::make_unique<Model>("res/models/nanosuit/nanosuit.obj");
	}

	Test10::~Test10() = default;

	void Test10::OnUpdate(float time, float deltaTime)
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

	void Test10::OnRender()
	{
		Renderer renderer;
		
		glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, 1.0f);
		
		glm::mat4 projection = glm::perspective(glm::radians(m_Camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = m_Camera->GetViewMatrix();

		m_Shader->Bind();

		// Set light shader values
		m_Shader->SetUniform3f("viewPos", m_Camera->Position);
		for (uint32_t i = 0; i < 2; i++)
		{
			if (m_PointLight[i].active)
			{
				m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].position", m_PointLight[i].position);
				m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].ambient", m_PointLight[i].ambient);
				m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].diffuse", m_PointLight[i].diffuse);
				m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].specular", m_PointLight[i].specular);
			}
			else
			{
				m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].position", glm::vec3());
				m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].ambient", glm::vec3());
				m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3());
				m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].specular", glm::vec3());
			}
			m_Shader->SetUniform1f("pointLights[" + std::to_string(i) + "].constant", m_PointLight[i].constant);
			m_Shader->SetUniform1f("pointLights[" + std::to_string(i) + "].linear", m_PointLight[i].linear);
			m_Shader->SetUniform1f("pointLights[" + std::to_string(i) + "].quadratic", m_PointLight[i].quadratic);
		}

		m_Shader->SetUniform1f("material.shininess", 32.0f);
		m_Shader->SetUniformMat4f("view", view);
		m_Shader->SetUniformMat4f("projection", projection);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		m_Shader->SetUniformMat4f("model", model);
		renderer.Draw(*m_NanosuitModel, *m_Shader);
		m_Shader->Unbind();
		
		// Render cube light
		m_VAO->Bind();
		m_LightShader->Bind();
		m_LightShader->SetUniformMat4f("view", m_Camera->GetViewMatrix());
		m_LightShader->SetUniformMat4f("projection", projection);
		for (uint32_t i = 0; i < 2; i++)
		{
			if (m_PointLight[i].active)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, m_PointLight[i].position);
				model = glm::scale(model, glm::vec3(0.2f));
				m_LightShader->SetUniformMat4f("model", model);
				renderer.Draw(*m_VAO, *m_IBO, *m_LightShader);
			}
		}
		m_LightShader->Unbind();
		
	}

	void Test10::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::ColorEdit3("ClearColor", &m_ClearColor[0]);

		for (int i = 0; i < 2; i++)
		{
			if (ImGui::CollapsingHeader(std::string("Point light " + std::to_string(i)).c_str()))
			{
				ImGui::Checkbox("Active", &m_PointLight[i].active);
				ImGui::SliderFloat3("Position", &m_PointLight[i].position[0], -30.0f, 30.0f);
				ImGui::ColorEdit3("Ambient", &m_PointLight[i].ambient[0]);
				ImGui::ColorEdit3("Diffuse", &m_PointLight[i].diffuse[0]);
				ImGui::ColorEdit3("Specular", &m_PointLight[i].specular[0]);
				ImGui::InputFloat("Constant", &m_PointLight[i].constant);
				ImGui::InputFloat("Linear", &m_PointLight[i].linear);
				ImGui::InputFloat("Quadratic", &m_PointLight[i].quadratic);
			}
		}
	}
}
