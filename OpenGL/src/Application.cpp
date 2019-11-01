#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "CustomGLCallback.h"
#include "Renderer.h"

#include "tests/TestMenu.h"
#include "tests/Test1.h"
#include "tests/Test2.h"
#include "tests/Test3.h"
#include "tests/Test4.h"
#include "tests/Test5.h"
#include "tests/Test6.h"
#include "tests/Test7.h"
#include "tests/Test8.h"
#include "tests/Test9.h"
#include "tests/Test10.h"
#include "tests/Test11.h"
#include "tests/Test12.h"
#include "tests/Test13.h"
#include "tests/Test14.h"
#include "tests/Test15.h"
#include "tests/Test16.h"
#include "tests/Test17.h"
#include "tests/Test18.h"
#include "tests/Test19.h"
#include "tests/Test20.h"
#include "tests/Test21.h"

// settings
extern const unsigned int SCR_WIDTH = 1920;
extern const unsigned int SCR_HEIGHT = 1080;


// enable optimus! => Use of nvidia card
//extern "C" {
//	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
//}




static int InitOpenGL(GLFWwindow** window)
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	*window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(*window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);
	glfwSetKeyCallback(*window, key_callback);
	glfwSetCursorPosCallback(*window, mouse_callback);
	glfwSetScrollCallback(*window, scroll_callback);

	glfwSwapInterval(0); // As fast as possible
	//glfwSwapInterval(1); // => 60 FPS

	std::cout << glGetString(GL_VERSION) << std::endl;
	std::cout << glGetString(GL_VENDOR) << std::endl;
	std::cout << glGetString(GL_RENDERER) << std::endl;

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	return 0;
}

int	main()
{
	GLFWwindow* window;

	if (InitOpenGL(&window) == -1)
		return -1;

	{
		Renderer renderer;
		renderer.EnableBlending();
		renderer.EnableDepthTesting();

		ImGui::CreateContext();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(nullptr);

		ImGui::StyleColorsDark();

		test::Test* currentTest = nullptr;
		test::TestMenu* menu = new test::TestMenu(currentTest);
		currentTest = menu;

		menu->RegisterTest<test::Test1>("Simple triangle rendering");
		menu->RegisterTest<test::Test2>("Mix two textures");
		menu->RegisterTest<test::Test3>("Transformations");
		menu->RegisterTest<test::Test4>("Camera");
		menu->RegisterTest<test::Test5>("Map camera to keyboard");
		menu->RegisterTest<test::Test6>("Point light with Phong lighting model");
		menu->RegisterTest<test::Test7>("Material + change light color over time");
		menu->RegisterTest<test::Test8>("Light map");
		menu->RegisterTest<test::Test9>("Directional, point and spot lights");
		menu->RegisterTest<test::Test10>("3D model loading + lighting");
		menu->RegisterTest<test::Test11>("Depth buffer visualization with Sponza");
		menu->RegisterTest<test::Test12>("Outline using stencil buffer (need to fix imgui on this one)");
		menu->RegisterTest<test::Test13>("Blending");
		menu->RegisterTest<test::Test14>("Backface culling");
		menu->RegisterTest<test::Test15>("Postprocessing");
		menu->RegisterTest<test::Test16>("Cubemap (skybox + environment mapping)");
		menu->RegisterTest<test::Test17>("Normal vector visualization + exploding object using geometry shader");
		menu->RegisterTest<test::Test18>("Asteroid field instancing");
		menu->RegisterTest<test::Test19>("Multisample anti-aliasing (MSAA)");
		menu->RegisterTest<test::Test20>("Shadow mapping");

		float deltaTime = 0.0f;	// Time between current frame and last frame
		float lastFrame = 0.0f; // Time of last frame

		//render loop
		while (!glfwWindowShouldClose(window))
		{
			//rendering
			renderer.Clear();

			const float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
		
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			if (currentTest)
			{
				currentTest->OnUpdate(currentFrame, deltaTime);
				currentTest->OnRender();
				ImGui::Begin("test");
				if (currentTest != menu && ImGui::Button("<-"))
				{
					delete currentTest;
					currentTest = menu;
				}
				currentTest->OnImGuiRender();
				ImGui::End();
			}
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Events and buffer swap
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}
