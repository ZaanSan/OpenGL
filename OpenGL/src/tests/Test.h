#pragma once

namespace  test
{
	class Test
	{
	public:
		Test() = default;
		virtual ~Test() = default;

		virtual void OnUpdate(float time, float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};
}