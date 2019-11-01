#include "CustomGLCallback.h"

#include "InputSystem.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputSystem& is = InputSystem::Instance();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	is.SetKeyState(key, action);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	InputSystem& is = InputSystem::Instance();

	is.SetMousePosition({ static_cast<float>(xpos), static_cast<float>(ypos) });
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	InputSystem& is = InputSystem::Instance();

	is.SetScrollOffset({ static_cast<float>(xoffset), static_cast<float>(yoffset) });
}