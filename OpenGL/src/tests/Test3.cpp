#include "Test3.h"

#include <iostream>

#include "imgui/imgui.h"

#include "../VertexBufferLayout.h"

namespace test
{

	Test3::Test3()
		: m_MixVal(0.2f), m_Translation(0.0f), m_Angle(0.0f), m_Scale(1.0f)
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

		m_Shader = std::make_unique<Shader>("res/shaders/Test3/vertex.glsl", "res/shaders/Test3/fragment.glsl");
		m_Texture = std::make_unique<Texture>("res/textures/container.jpg");
		m_Texture2 = std::make_unique<Texture>("res/textures/awesomeface.png");
		m_Shader->SetUniform1i("texture1", 0);
		m_Shader->SetUniform1i("texture2", 1);

		
	}

	Test3::~Test3() = default;

	void Test3::OnUpdate(float time, float deltaTime)
	{
	}

	void Test3::OnRender()
	{
		Renderer renderer;

		glm::mat4 trans = glm::mat4(1.0f); // Identity matrix == glm::mat4();
		trans = glm::translate(trans, m_Translation);
		trans = glm::rotate(trans, glm::radians(m_Angle), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(m_Scale));
		m_Shader->SetUniformMat4f("transform", trans);

		m_Shader->SetUniform1f("mixVal", m_MixVal);

		m_Texture->Bind(0);
		m_Texture2->Bind(1);
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
		m_Texture->Unbind();
		m_Texture2->Unbind();
	}

	void Test3::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderFloat("Mix value", &m_MixVal, 0.0f, 1.0f);
		ImGui::SliderFloat("Rotation", &m_Angle, 0.0f, 360.0f);
		ImGui::SliderFloat("Scale factor", &m_Scale, 0.0f, 10.0f);
		ImGui::SliderFloat3("Translation", &m_Translation[0], -1.0f, 1.0f);
	}

}
