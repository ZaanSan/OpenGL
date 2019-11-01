#include "Test13.h"

#include <algorithm>

#include "imgui/imgui.h"

#include "../VertexBufferLayout.h"
#include "../InputSystem.h"

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

namespace test
{

	Test13::Test13()
		: m_Time(0.0f),
		m_ClearColor(glm::vec3(0.0f))
	{

		float vertices[] = {
			// positions         // texture coords
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
								 
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
								 
			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
								 
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
								 
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
								 
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
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
		VertexBufferLayout layoutPlane;
		layoutPlane.Push<float>(3);
		layoutPlane.Push<float>(2);
		m_IBOPlane = std::make_unique<IndexBuffer>(planeIndices, 6);
		m_VAOPlane->AddBuffer(*m_VBOPlane, layout);
		m_VAOPlane->Unbind();
		m_IBOPlane->Unbind();

		// window VAO => Same IBO as plane because.. it's a plane !
		m_VAOwindow = std::make_unique<VertexArray>();
		m_VAOwindow->Bind();
		m_VBOwindow = std::make_unique<VertexBuffer>(windowVertices, sizeof(windowVertices));
		VertexBufferLayout layoutwindow;
		layoutwindow.Push<float>(3);
		layoutwindow.Push<float>(2);
		m_IBOPlane->Bind();
		m_VAOwindow->AddBuffer(*m_VBOwindow, layout);
		m_VAOwindow->Unbind();
		m_IBOPlane->Unbind();

		m_windowCoordinates.emplace_back(glm::vec3(-1.5f, 0.0f, -0.48f));
		m_windowCoordinates.emplace_back(glm::vec3(1.5f, 0.0f, 0.51f));
		m_windowCoordinates.emplace_back(glm::vec3(0.0f, 0.0f, 0.7f));
		m_windowCoordinates.emplace_back(glm::vec3(-0.3f, 0.0f, -2.3f));
		m_windowCoordinates.emplace_back(glm::vec3(0.5f, 0.0f, -0.6f));

		m_Shader = std::make_unique<Shader>("res/shaders/Test13/vertex.glsl", "res/shaders/Test13/fragment.glsl");
		m_Shader->Unbind();

		m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

		m_CubeTexture = std::make_unique<Texture>("res/textures/marble.jpg");
		m_PlaneTexture = std::make_unique<Texture>("res/textures/metal.jpg");
		m_windowTexture = std::make_unique<Texture>("res/textures/blending_transparent_window.png", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	}

	Test13::~Test13() = default;

	void Test13::OnUpdate(float time, float deltaTime)
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

	void Test13::OnRender()
	{
		Renderer renderer;

		glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, 1.0f);

		glm::mat4 projection = glm::perspective(glm::radians(m_Camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = m_Camera->GetViewMatrix();

		m_Shader->Bind();

		// Set shader values
		m_Shader->SetUniformMat4f("view", view);
		m_Shader->SetUniformMat4f("projection", projection);

		// Draw 2 cubes
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

		// Draw plane
		m_VAOPlane->Bind();
		m_PlaneTexture->Bind(0);
		model = glm::mat4(1.0f);
		m_Shader->SetUniformMat4f("model", model);
		renderer.Draw(*m_VAOPlane, *m_IBOPlane, *m_Shader);
		m_PlaneTexture->Unbind();
		m_VAOPlane->Unbind();

		// Sort transparent object
		std::sort(m_windowCoordinates.begin(), m_windowCoordinates.end(),
			[this](const glm::vec3& pos1, const glm::vec3& pos2) {
			return glm::length2(m_Camera->Position - pos1) >= glm::length2(m_Camera->Position - pos2);
		});

		// Draw window from farthest to nearest
		m_VAOwindow->Bind();
		m_windowTexture->Bind(0);
		for (auto& windowCoordinate : m_windowCoordinates)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, windowCoordinate);
			m_Shader->SetUniformMat4f("model", model);
			renderer.Draw(*m_VAOwindow, *m_IBOPlane, *m_Shader);
		}
		m_windowTexture->Unbind();
		m_VAOwindow->Unbind();
	
	}

	void Test13::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::ColorEdit3("ClearColor", &m_ClearColor[0]);
	}
}
