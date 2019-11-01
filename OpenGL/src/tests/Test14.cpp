#include "Test14.h"

#include <algorithm>

#include "imgui/imgui.h"

#include "../VertexBufferLayout.h"
#include "../InputSystem.h"
#include "../TextureManager.h"

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

namespace test
{

	Test14::Test14()
		: m_Time(0.0f),
		m_ClearColor(glm::vec3(0.0f))
	{

		float vertices[] = {
			// Back face
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
			// Front face
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
			// Left face
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
			// Right face
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
			// Bottom face
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
			// Top face
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left  
		};

		unsigned int indices[] = {
			0, 1, 2,
			1, 0, 3,
			4, 5, 6,
			6, 7, 4,
			8, 9, 10,
			10, 11, 8,
			12, 13, 14,
			13, 12, 15,
			16, 17, 18,
			18, 19, 16,
			20, 21, 22,
			21, 20, 23
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

		m_Shader = std::make_unique<Shader>("res/shaders/Test14/vertex.glsl", "res/shaders/Test14/fragment.glsl");
		m_Shader->Unbind();

		m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

		m_CubeTexture = std::make_unique<Texture>("res/textures/marble.jpg");
		m_PlaneTexture = std::make_unique<Texture>("res/textures/metal.jpg");
	}

	Test14::~Test14() = default;

	void Test14::OnUpdate(float time, float deltaTime)
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

	void Test14::OnRender()
	{
		Renderer renderer;

		glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, 1.0f);

		glm::mat4 projection = glm::perspective(glm::radians(m_Camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = m_Camera->GetViewMatrix();

		m_Shader->Bind();

		// Set shader values
		m_Shader->SetUniformMat4f("view", view);
		m_Shader->SetUniformMat4f("projection", projection);

		// Draw 2 cubes : Backface culling
		renderer.EnableBackfaceCulling();
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
		renderer.DisableBackfaceCulling();

		// Draw plane
		m_VAOPlane->Bind();
		m_PlaneTexture->Bind(0);
		model = glm::mat4(1.0f);
		m_Shader->SetUniformMat4f("model", model);
		renderer.Draw(*m_VAOPlane, *m_IBOPlane, *m_Shader);
		m_PlaneTexture->Unbind();
		m_VAOPlane->Unbind();
	}

	void Test14::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::ColorEdit3("ClearColor", &m_ClearColor[0]);
	}
}
