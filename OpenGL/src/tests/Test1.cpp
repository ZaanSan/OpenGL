#include "Test1.h"

#include "../VertexBufferLayout.h"

namespace test
{

	Test1::Test1()
	{
		float vertices[] = {
			// positions		  // colors
			 0.5f, -0.5f,  0.0f,  1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f,
			 0.0f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 2 
		};

		m_VAO = std::make_unique<VertexArray>();
		m_VAO->Bind();

		m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);

		m_IBO = std::make_unique<IndexBuffer>(indices, 3);

		m_VAO->AddBuffer(*m_VBO, layout);

		m_Shader = std::make_unique<Shader>("res/shaders/Test1/vertex.glsl", "res/shaders/Test1/fragment.glsl");

	}

	Test1::~Test1() = default;

	void Test1::OnUpdate(float time, float deltaTime)
	{

	}

	void Test1::OnRender()
	{
		Renderer renderer;
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
	}

	void Test1::OnImGuiRender()
	{

	}

}