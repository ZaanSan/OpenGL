#include "Test18.h"

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

	Test18::Test18()
		: m_Time(0.0f),
		m_ClearColor(glm::vec3(0.0f))
	{
		srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed	
		float radius = 50.0;
		float offset = 2.5f;
		for (unsigned int i = 0; i < amount; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			// 1. translation: displace along circle with 'radius' in range [-offset, offset]
			float angle = (float)i / (float)amount * 360.0f;
			float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float x = sin(angle) * radius + displacement;
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float z = cos(angle) * radius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));

			// 2. scale: Scale between 0.05 and 0.25f
			float scale = (rand() % 20) / 100.0f + 0.05f;
			model = glm::scale(model, glm::vec3(scale));

			// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
			float rotAngle = (float)(rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

			// 4. now add to list of matrices
			modelMatrices[i] = model;
		}

		m_Planet = std::make_unique<Model>("res/models/planet/planet.obj");
		m_Rock = std::make_unique<Model>("res/models/rock/rock.obj");

		m_VBOInstanced = std::make_unique<VertexBuffer>(&modelMatrices[0], amount * sizeof(glm::mat4));
		VertexBufferLayout layout;
		layout.Push<float>(4);
		layout.Push<float>(4);
		layout.Push<float>(4);
		layout.Push<float>(4);
		m_Rock->AddInstancedArray(*m_VBOInstanced, layout, 3);

		m_PlanetShader = std::make_unique<Shader>("res/shaders/Test18/vertex.glsl", "res/shaders/Test18/fragment.glsl");
		m_RockShader = std::make_unique<Shader>("res/shaders/Test18/rock_vertex.glsl", "res/shaders/Test18/rock_fragment.glsl");

		m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 55.0f));
	}

	Test18::~Test18() = default;

	void Test18::OnUpdate(float time, float deltaTime)
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

	void Test18::OnRender()
	{
		Renderer renderer;

		glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, 1.0f);

		glm::mat4 projection = glm::perspective(glm::radians(m_Camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = m_Camera->GetViewMatrix();
		m_RockShader->Bind();
		m_RockShader->SetUniformMat4f("projection", projection);
		m_RockShader->SetUniformMat4f("view", view);
		m_PlanetShader->Bind();
		m_PlanetShader->SetUniformMat4f("projection", projection);
		m_PlanetShader->SetUniformMat4f("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		m_PlanetShader->SetUniformMat4f("model", model);
		renderer.Draw(*m_Planet, *m_PlanetShader);

		renderer.DrawInstanced(*m_Rock, *m_RockShader, amount);
	}

	void Test18::OnImGuiRender()
	{
		static int effect_idx = 0;

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
