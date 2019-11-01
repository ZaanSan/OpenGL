#include "Test8.h"

#include "imgui/imgui.h"

#include "../VertexBufferLayout.h"
#include "../InputSystem.h"

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

namespace test
{

	Test8::Test8()
		: m_Time(0.0f),
		m_LightAmbient(0.2f, 0.2f, 0.2f),
		m_LightDiffuse(0.5f, 0.5f, 0.5f),
		m_LightSpecular(1.0f, 1.0f, 1.0f)
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

		m_VAO = std::make_unique<VertexArray>();
		m_VAO->Bind();

		m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_IBO = std::make_unique<IndexBuffer>(indices, 36);
		m_VAO->AddBuffer(*m_VBO, layout);
		m_Shader = std::make_unique<Shader>("res/shaders/Test8/vertex.glsl", "res/shaders/Test8/fragment.glsl");
		m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
		m_Shader->Unbind();

		m_LightShader = std::make_unique<Shader>("res/shaders/Test8/vertex.glsl", "res/shaders/Test8/lightFragment.glsl");
		m_LightShader->Unbind();

		m_DiffuseMap = std::make_unique<Texture>("res/textures/container2.png");
		m_SpecularMap = std::make_unique<Texture>("res/textures/container2_specular.png");
		m_EmissionMap = std::make_unique<Texture>("res/textures/matrix.jpg");

		m_VAO->Unbind();
	}

	Test8::~Test8() = default;

	void Test8::OnUpdate(float time, float deltaTime)
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

	void Test8::OnRender()
	{
		Renderer renderer;
		glm::mat4 projection = glm::perspective(glm::radians(m_Camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = m_Camera->GetViewMatrix();

		m_VAO->Bind();

		// Render cube
		m_DiffuseMap->Bind(0);
		m_SpecularMap->Bind(1);
		m_EmissionMap->Bind(2);
		m_Shader->Bind();
		m_Shader->SetUniform3f("light.position", m_LightPositions[0]);
		m_Shader->SetUniform3f("light.ambient", m_LightAmbient);
		m_Shader->SetUniform3f("light.diffuse", m_LightDiffuse);
		m_Shader->SetUniform3f("light.specular", m_LightSpecular);
		m_Shader->SetUniform1i("material.diffuse", 0);
		m_Shader->SetUniform1i("material.specular", 1);
		m_Shader->SetUniform1i("material.emission", 2);
		m_Shader->SetUniform1f("material.shininess", 32.0f);
		m_Shader->SetUniformMat4f("view", m_Camera->GetViewMatrix());
		m_Shader->SetUniformMat4f("projection", projection);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, m_CubePositions[0]);
		m_Shader->SetUniformMat4f("model", model);
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
		m_Shader->Unbind();
		m_DiffuseMap->Unbind();
		m_SpecularMap->Unbind();
		m_EmissionMap->Unbind();

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

	void Test8::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::NewLine();
		ImGui::Text("Light :");
		ImGui::SliderFloat3("Ambient", &m_LightAmbient[0], 0.0f, 1.0f);
		ImGui::SliderFloat3("Diffuse", &m_LightDiffuse[0], 0.0f, 1.0f);
		ImGui::SliderFloat3("Specular", &m_LightSpecular[0], 0.0f, 1.0f);
	}
}
