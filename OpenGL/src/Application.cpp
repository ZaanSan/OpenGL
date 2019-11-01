#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "CustomGLCallback.h"
#include "Renderer.h"

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

/*
// enable optimus! => Use of nvidia card
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
*/



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

		test::Test10 test;

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
			test.OnUpdate(currentFrame, deltaTime);
			test.OnRender();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			test.OnImGuiRender();
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
