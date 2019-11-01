#include "Test12.h"

#include "imgui/imgui.h"

#include "../VertexBufferLayout.h"
#include "../InputSystem.h"

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

namespace test
{

	Test12::Test12()
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
		m_LightShader = std::make_unique<Shader>("res/shaders/Test12/vertex.glsl", "res/shaders/Test12/lightFragment.glsl");
		m_LightShader->Unbind();
		m_VAO->Unbind();

		m_Shader = std::make_unique<Shader>("res/shaders/Test12/vertex.glsl", "res/shaders/Test12/fragment.glsl");
		m_Shader->Unbind();
		m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
		m_Sponza = std::make_unique<Model>("res/models/sponza/sponza.obj");
		m_NanosuitModel = std::make_unique<Model>("res/models/nanosuit/nanosuit.obj");
		m_OutlineShader = std::make_unique<Shader>("res/shaders/Test12/vertex.glsl", "res/shaders/Test12/shaderSingleColor.glsl");
		m_OutlineShader->Unbind();
	}

	Test12::~Test12() = default;

	void Test12::OnUpdate(float time, float deltaTime)
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

	void Test12::OnRender()
	{
		Renderer renderer;

		glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, 1.0f);

		glm::mat4 projection = glm::perspective(glm::radians(m_Camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = m_Camera->GetViewMatrix();

		glStencilMask(0x00);

		m_Shader->Bind();

		// Set light shader values
		m_Shader->SetUniform3f("viewPos", m_Camera->Position);

		if (m_DirLight.active)
		{
			m_Shader->SetUniform3f("dirLight.direction", m_DirLight.direction);
			m_Shader->SetUniform3f("dirLight.ambient", m_DirLight.ambient);
			m_Shader->SetUniform3f("dirLight.diffuse", m_DirLight.diffuse);
			m_Shader->SetUniform3f("dirLight.specular", m_DirLight.specular);
		}
		else
		{
			m_Shader->SetUniform3f("dirLight.direction", glm::vec3());
			m_Shader->SetUniform3f("dirLight.ambient", glm::vec3());
			m_Shader->SetUniform3f("dirLight.diffuse", glm::vec3());
			m_Shader->SetUniform3f("dirLight.specular", glm::vec3());
		}

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
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));	// it's a bit too big for our scene, so scale it down
		model = glm::scale(model, glm::vec3(0.02f));	// it's a bit too big for our scene, so scale it down
		m_Shader->SetUniformMat4f("model", model);
		renderer.Draw(*m_Sponza, *m_Shader);
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
		m_VAO->Unbind();

		// Outline object

		renderer.EnableStencilTesting();
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should update the stencil buffer
		glStencilMask(0xFF); // enable writing to the stencil buffer

		m_Shader->Bind();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f));	// it's a bit too big for our scene, so scale it down
		m_Shader->SetUniformMat4f("model", model);
		renderer.Draw(*m_NanosuitModel, *m_Shader);
		m_Shader->Unbind();

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00); // disable writing to the stencil buffer
		renderer.DisableDepthTesting();

		m_OutlineShader->Bind();
		m_OutlineShader->SetUniformMat4f("view", view);
		m_OutlineShader->SetUniformMat4f("projection", projection);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.85f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.21f));	// it's a bit too big for our scene, so scale it down
		m_OutlineShader->SetUniformMat4f("model", model);
		renderer.Draw(*m_NanosuitModel, *m_OutlineShader);
		m_OutlineShader->Unbind();
		glStencilMask(0xFF);
		renderer.EnableDepthTesting();


	}

	void Test12::OnImGuiRender()
	{
		//TODO Fix ImGui => Stencil issue
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::ColorEdit3("ClearColor", &m_ClearColor[0]);

		if (ImGui::CollapsingHeader("Directional light"))
		{
			ImGui::Checkbox("Active", &m_DirLight.active);
			ImGui::SliderFloat3("Direction", &m_DirLight.direction[0], -10.0f, 10.0f);
			ImGui::ColorEdit3("Ambient", &m_DirLight.ambient[0]);
			ImGui::ColorEdit3("Diffuse", &m_DirLight.diffuse[0]);
			ImGui::ColorEdit3("Specular", &m_DirLight.specular[0]);
		}

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
