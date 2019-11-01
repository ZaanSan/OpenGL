#include "Test21.h"

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
	Test21::Test21()
		: m_Time(0.0f),
		m_ClearColor(glm::vec3(0.0f))
	{
		float vertices[] = {
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
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
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
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
			// positions            // normals         // texcoords
			 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
			-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
			-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
			 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f,
		};

		unsigned int planeIndices[] = {
			0, 3, 1,
			1, 3, 2
		};

		float quadVertices[] = {
			// positions        // texture Coords
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);

		VertexBufferLayout debugLayout;
		debugLayout.Push<float>(3);
		debugLayout.Push<float>(2);

		m_Cube = std::make_unique<Shape>(vertices, sizeof(vertices), layout, indices, 36);
		m_Plane = std::make_unique<Shape>(planeVertices, sizeof(planeVertices), layout, planeIndices, 6);

		m_WoodTexture = std::make_unique<Texture>("res/textures/wood.png");

		m_DepthMapFBO = std::make_unique<FrameBuffer>();
		m_DepthMap = std::make_unique<Cubemap>(1024, 1024);
		m_DepthMap->BindAsDepthAttachment(*m_DepthMapFBO);

		m_Shader = std::make_unique<Shader>("res/shaders/Test21/vertex.glsl", "res/shaders/Test21/fragment.glsl");
		m_DepthShader = std::make_unique<Shader>("res/shaders/Test21/simple_depth_vertex.glsl", "res/shaders/Test21/simple_depth_fragment.glsl");

		m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
	}

	Test21::~Test21() = default;

	void Test21::OnUpdate(float time, float deltaTime)
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
	};

	void Test21::OnRender()
	{
		Renderer renderer;


		glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, 1.0f);

		float near_plane = 1.0f, far_plane = 7.5f;
		glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		glm::mat4 projection = glm::perspective(glm::radians(m_Camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = m_Camera->GetViewMatrix();

		renderer.EnableBackfaceCulling();
		renderer.SetViewport(0, 0, 1024, 1024);
		m_DepthMapFBO->Bind();
		renderer.Clear();
		m_DepthShader->Bind();
		m_DepthShader->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
		m_WoodTexture->Bind(0);
		RenderScene(*m_DepthShader);
		m_DepthShader->Unbind();
		m_DepthMap->Unbind();

		renderer.SetViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		renderer.Clear();
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("projection", projection);
		m_Shader->SetUniformMat4f("view", view);
		m_Shader->SetUniform1i("diffuseTexture", 0);
		m_WoodTexture->Bind(0);
		m_Shader->SetUniform1i("shadowMap", 1);
		m_DepthMap->Bind(0);
		// Set light uniforms
		m_Shader->SetUniform3f("viewPos", m_Camera->Position);
		m_Shader->SetUniform3f("lightPos", lightPos);
		m_Shader->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
		RenderScene(*m_Shader);
	}

	void Test21::OnImGuiRender()
	{
		static int effect_idx = 0;

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	void Test21::RenderScene(Shader& shader)
	{
		Renderer renderer;

		// floor
		glm::mat4 model = glm::mat4(1.0f);
		shader.SetUniformMat4f("model", model);
		renderer.Draw(*m_Plane, shader);
		// cubes
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
		//model = glm::scale(model, glm::vec3(0.5f));
		shader.SetUniformMat4f("model", model);
		renderer.Draw(*m_Cube, shader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
		//model = glm::scale(model, glm::vec3(0.5f));
		shader.SetUniformMat4f("model", model);
		renderer.Draw(*m_Cube, shader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(0.25));
		shader.SetUniformMat4f("model", model);
		renderer.Draw(*m_Cube, shader);
	}
}
