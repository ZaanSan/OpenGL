#include "Test4.h"

#include "imgui/imgui.h"

#include "../VertexBufferLayout.h"

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

namespace test
{

	Test4::Test4()
		: m_MixVal(0.2f),
		m_Time(0.0f),
		m_FOV(45.0f),
		m_AspectRatio(static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT)),
		m_CameraTranslate(0.0f, 0.0f, -5.0f)
	{
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
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
		layout.Push<float>(2);

		m_IBO = std::make_unique<IndexBuffer>(indices, 36);

		m_VAO->AddBuffer(*m_VBO, layout);

		m_Shader = std::make_unique<Shader>("res/shaders/Test4/vertex.glsl", "res/shaders/Test4/fragment.glsl");
		m_Texture = std::make_unique<Texture>("res/textures/container.jpg");
		m_Texture2 = std::make_unique<Texture>("res/textures/awesomeface.png");
		m_Shader->SetUniform1i("texture1", 0);
		m_Shader->SetUniform1i("texture2", 1);	
	}

	Test4::~Test4() = default;

	void Test4::OnUpdate(float time, float deltaTime)
	{
		m_Time = time;
		m_Shader->SetUniform1f("mixVal", m_MixVal);
	}

	void Test4::OnRender()
	{
		Renderer renderer;

		m_Texture->Bind(0);
		m_Texture2->Bind(1);

		m_VAO->Bind();

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, m_CubePositions[i]);
			model = glm::rotate(model, (i * 0.3f) * m_Time * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

			glm::mat4 view = glm::mat4(1.0f);
			// note that we're translating the scene in the reverse direction of where we want to move
			view = glm::translate(view, m_CameraTranslate);

			glm::mat4 projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, 0.1f, 100.0f);

			m_Shader->SetUniformMat4f("model", model);
			m_Shader->SetUniformMat4f("view", view);
			m_Shader->SetUniformMat4f("projection", projection);
			renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
		}

		m_Texture->Unbind();
		m_Texture2->Unbind();
	}

	void Test4::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderFloat("Mix value", &m_MixVal, 0.0f, 1.0f);
		ImGui::SliderFloat("FOV", &m_FOV, 0.0f, 180.0f);
		ImGui::SliderFloat("Aspect ratio", &m_AspectRatio, 0.0f, 10.0f);
		ImGui::SliderFloat3("Camera translate", &m_CameraTranslate[0], -100.0f, 100.0f);
	}

}
