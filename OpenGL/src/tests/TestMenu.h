#pragma once

#include <memory>
#include <vector>
#include <string>
#include <functional>

#include "Test.h"

namespace test
{
	class TestMenu : public test::Test
	{
	private:
		Test*& m_CurrentTestPointer;
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;


	public:
		TestMenu(Test*& currentTestPointer);
		~TestMenu();

		template<typename T>
		void RegisterTest(std::string&& name)
		{
			std::cout << "Registering test " << name << std::endl;
			m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
		};
		
		void OnUpdate(float time, float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
