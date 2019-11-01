#include "Test9.h"

#include "imgui/imgui.h"

#include "../VertexBufferLayout.h"
#include "../InputSystem.h"

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

namespace test
{

	Test9::Test9()
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

		m_VAO = std::make_unique<VertexArray>();
		m_VAO->Bind();

		m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_IBO = std::make_unique<IndexBuffer>(indices, 36);
		m_VAO->AddBuffer(*m_VBO, layout);

		// Directional light
		//m_Shader = std::make_unique<Shader>("res/shaders/Test9/vertex.glsl", "res/shaders/Test9/DirectionalLightFragment.glsl");
		
		// Point light
		//m_Shader = std::make_unique<Shader>("res/shaders/Test9/vertex.glsl", "res/shaders/Test9/PointLightFragment.glsl");

		// Spot light
		//m_Shader = std::make_unique<Shader>("res/shaders/Test9/vertex.glsl", "res/shaders/Test9/SpotLightFragment.glsl");

		// 6 Lights (1 dir, 4 point, 1 spot)
		m_Shader = std::make_unique<Shader>("res/shaders/Test9/vertex.glsl", "res/shaders/Test9/fragment.glsl");

		m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
		m_Shader->Unbind();

		m_LightShader = std::make_unique<Shader>("res/shaders/Test9/vertex.glsl", "res/shaders/Test9/lightFragment.glsl");
		m_LightShader->Unbind();

		m_DiffuseMap = std::make_unique<Texture>("res/textures/container2.png");
		m_SpecularMap = std::make_unique<Texture>("res/textures/container2_specular.png");

		m_VAO->Unbind();
	}

	Test9::~Test9() = default;

	void Test9::OnUpdate(float time, float deltaTime)
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

	void Test9::OnRender()
	{
		Renderer renderer;
		glm::mat4 projection = glm::perspective(glm::radians(m_Camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = m_Camera->GetViewMatrix();

		glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, 1.0f);

		m_VAO->Bind();

		// Render cube
		m_DiffuseMap->Bind(0);
		m_SpecularMap->Bind(1);
		m_Shader->Bind();

		m_Shader->SetUniform3f("viewPos", m_Camera->Position);

		// Directional light
		//m_Shader->SetUniform3f("light.direction", -0.2f, -1.0f, -0.3f);
		
		// Point light
		//m_Shader->SetUniform3f("light.position", 1.2f,  1.0f,  2.0f);
		//m_Shader->SetUniform1f("light.constant", 1.0f);
		//m_Shader->SetUniform1f("light.linear", 0.09f);
		//m_Shader->SetUniform1f("light.quadratic", 0.032f);

		// Spot light
		//m_Shader->SetUniform3f("light.position", m_Camera->Position);
		//m_Shader->SetUniform3f("light.direction", m_Camera->Front);
		//m_Shader->SetUniform1f("light.cutOff", glm::cos(glm::radians(12.5f)));
		//m_Shader->SetUniform1f("light.outerCutOff", glm::cos(glm::radians(17.5f)));

		// Use these values for Direction light, Point light or Spot light alone
		//m_Shader->SetUniform3f("light.ambient", (0.2f, 0.2f, 0.2f);
		//m_Shader->SetUniform3f("light.diffuse", 0.5f, 0.5f, 0.5f);
		//m_Shader->SetUniform3f("light.specular", 1.0f, 1.0f, 1.0f);


		// 6 lights
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

		for (uint32_t i = 0; i < 4; i++)
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

		if (m_SpotLight.active)
		{
			m_SpotLight.position = m_Camera->Position;
			m_SpotLight.direction = m_Camera->Front;

			m_Shader->SetUniform3f("spotLight.position", m_SpotLight.position);
			m_Shader->SetUniform3f("spotLight.direction", m_SpotLight.direction);
			m_Shader->SetUniform3f("spotLight.ambient", m_SpotLight.ambient);
			m_Shader->SetUniform3f("spotLight.diffuse", m_SpotLight.diffuse);
			m_Shader->SetUniform3f("spotLight.specular", m_SpotLight.specular);
			m_Shader->SetUniform1f("spotLight.cutOff", m_SpotLight.cutOff);
			m_Shader->SetUniform1f("spotLight.outerCutOff", m_SpotLight.outerCutOff);
		}
		else
		{
			m_Shader->SetUniform3f("spotLight.position", glm::vec3());
			m_Shader->SetUniform3f("spotLight.direction", glm::vec3());
			m_Shader->SetUniform3f("spotLight.ambient", glm::vec3());
			m_Shader->SetUniform3f("spotLight.diffuse", glm::vec3());
			m_Shader->SetUniform3f("spotLight.specular", glm::vec3());
		}

		m_Shader->SetUniform1i("material.diffuse", 0);
		m_Shader->SetUniform1i("material.specular", 1);
		m_Shader->SetUniform1f("material.shininess", 32.0f);
		m_Shader->SetUniformMat4f("view", m_Camera->GetViewMatrix());
		m_Shader->SetUniformMat4f("projection", projection);

		glm::mat4 model = glm::mat4(1.0f);
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, m_CubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			m_Shader->SetUniformMat4f("model", model);

			renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
		}
		m_Shader->Unbind();
		m_DiffuseMap->Unbind();
		m_SpecularMap->Unbind();


		// Render light (Only use for Point Light)
		//m_LightShader->Bind();
		//m_LightShader->SetUniformMat4f("view", m_Camera->GetViewMatrix());
		//m_LightShader->SetUniformMat4f("projection", projection);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, m_LightPositions[0]);
		//model = glm::scale(model, glm::vec3(0.2f));
		//m_LightShader->SetUniformMat4f("model", model);
		//renderer.Draw(*m_VAO, *m_IBO, *m_LightShader);
		//m_LightShader->Unbind();

		// Render light (Only use for 6 Light)
		m_LightShader->Bind();
		m_LightShader->SetUniformMat4f("view", m_Camera->GetViewMatrix());
		m_LightShader->SetUniformMat4f("projection", projection);
		for (auto& i : m_PointLight)
		{
			if (i.active)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, i.position);
				model = glm::scale(model, glm::vec3(0.2f));
				m_LightShader->SetUniformMat4f("model", model);
				renderer.Draw(*m_VAO, *m_IBO, *m_LightShader);
			}
		}
		m_LightShader->Unbind();
	}

	void Test9::OnImGuiRender()
	{
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
		for (int i = 0; i < 4; i++)
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
		if (ImGui::CollapsingHeader("Spot light"))
		{
			ImGui::Checkbox("Active", &m_SpotLight.active);
			ImGui::ColorEdit3("Ambient", &m_SpotLight.ambient[0]);
			ImGui::ColorEdit3("Diffuse", &m_SpotLight.diffuse[0]);
			ImGui::ColorEdit3("Specular", &m_SpotLight.specular[0]);
			ImGui::SliderFloat("Cut off", &m_SpotLight.cutOff, 0.0f, 1.0f);
			ImGui::SliderFloat("Outer cut off", &m_SpotLight.outerCutOff, 0.0f, 1.0f);
		}
	}
}
