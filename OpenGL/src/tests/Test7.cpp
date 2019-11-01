#include "Test7.h"

#include "imgui/imgui.h"

#include "../VertexBufferLayout.h"
#include "../InputSystem.h"

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

namespace test
{

	Test7::Test7()
		: m_Time(0.0f)
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
		m_Shader = std::make_unique<Shader>("res/shaders/Test7/vertex.glsl", "res/shaders/Test7/fragment.glsl");
		m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
		m_Shader->Unbind();

		m_LightShader = std::make_unique<Shader>("res/shaders/Test7/vertex.glsl", "res/shaders/Test7/lightFragment.glsl");
		m_LightShader->Unbind();

		m_VAO->Unbind();
	}

	Test7::~Test7() = default;

	void Test7::OnUpdate(float time, float deltaTime)
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

	void Test7::OnRender()
	{
		Renderer renderer;
		glm::mat4 projection = glm::perspective(glm::radians(m_Camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = m_Camera->GetViewMatrix();

		// Change light color over time (Could be done on update function)
		glm::vec3 lightColor;
		lightColor.x = sin(m_Time * 2.0f);
		lightColor.y = sin(m_Time * 0.7f);
		lightColor.z = sin(m_Time * 1.3f);

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

		m_VAO->Bind();

		// Render cube
		m_Shader->Bind();
		m_Shader->SetUniform3f("light.position",	 m_LightPositions[0]);
		m_Shader->SetUniform3f("light.ambient",		 ambientColor);
		m_Shader->SetUniform3f("light.diffuse",		 diffuseColor);
		m_Shader->SetUniform3f("light.specular",     1.0f, 1.0f, 1.0f);
		m_Shader->SetUniform3f("material.ambient",   1.0f, 0.5f, 0.31f);
		m_Shader->SetUniform3f("material.diffuse",   1.0f, 0.5f, 0.31f);
		m_Shader->SetUniform3f("material.specular",  0.5f, 0.5f, 0.5f);
		m_Shader->SetUniform1f("material.shininess", 32.0f);
		m_Shader->SetUniformMat4f("view",			 m_Camera->GetViewMatrix());
		m_Shader->SetUniformMat4f("projection",		 projection);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, m_CubePositions[0]);
		m_Shader->SetUniformMat4f("model",			 model);
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

	void Test7::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}
