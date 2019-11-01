#include "Test2.h"

#include "../VertexBufferLayout.h"
#include "imgui/imgui.h"

namespace test
{

	Test2::Test2()
		: m_MixVal(0.2f)
	{
		float vertices[] = {
			// positions          // colors           // texture coords
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
		};

		unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3
		};

		m_VAO = std::make_unique<VertexArray>();
		m_VAO->Bind();

		m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);

		m_IBO = std::make_unique<IndexBuffer>(indices, 6);

		m_VAO->AddBuffer(*m_VBO, layout);

		m_Shader = std::make_unique<Shader>("res/shaders/Test2/vertex.glsl", "res/shaders/Test2/fragment.glsl");
		m_Texture = std::make_unique<Texture>("res/textures/container.jpg");
		m_Texture2 = std::make_unique<Texture>("res/textures/awesomeface.png");
		m_Shader->SetUniform1i("texture1", 0);
		m_Shader->SetUniform1i("texture2", 1);
	}

	Test2::~Test2() = default;

	void Test2::OnUpdate(float time, float deltaTime)
	{
		m_Shader->SetUniform1f("mixVal", m_MixVal);
	}

	void Test2::OnRender()
	{
		Renderer renderer;

		m_Texture->Bind(0);
		m_Texture2->Bind(1);
		
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);

		m_Texture->Unbind();
		m_Texture2->Unbind();
	}

	void Test2::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderFloat("Mix value", &m_MixVal, 0.0f, 1.0f);
	}

}
