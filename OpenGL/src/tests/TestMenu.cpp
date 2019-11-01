#include "TestMenu.h"

#include <iostream>
#include "imgui/imgui.h"

test::TestMenu::TestMenu(Test*& currentTestPointer)
	: m_CurrentTestPointer(currentTestPointer)
{
}

test::TestMenu::~TestMenu() = default;

void test::TestMenu::OnUpdate(float time, float deltaTime)
{
}

void test::TestMenu::OnRender()
{
}

void test::TestMenu::OnImGuiRender()
{
	for (auto& test : m_Tests)
	{
		if (ImGui::Button(test.first.c_str()))
		{
			m_CurrentTestPointer = test.second();
		}
	}
}
