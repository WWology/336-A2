#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Callbacks.h"

class Window
{

	GLFWwindow* m_Window;
	uint32_t m_Width;
	uint32_t m_Height;

public:
	Window(uint32_t t_Width, uint32_t t_Height)
		: m_Width(t_Width), m_Height(t_Height)
	{
		m_Window = initOpenGL();
	}

	~Window()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	GLFWwindow* getWindow() const { return m_Window; }
	uint32_t getWidth() const { return m_Width; }
	uint32_t getHeight() const { return m_Height; }

private:
	GLFWwindow* initOpenGL()
	{
		GLFWwindow* window;

		uint32_t WINDOW_WIDTH = 800;
		uint32_t WINDOW_HEIGHT = 800;

		glfwSetErrorCallback(error_callback);

		if (!glfwInit())
		{
			std::cerr << "Failed to initialise GLFW" << std::endl;
			exit(EXIT_FAILURE);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(m_Width, m_Height, "5281568_YosuaMartiansia_A2", nullptr, nullptr);

		if (window == NULL)
		{
			std::cerr << "Failed to create a window" << std::endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);

		if (glewInit() != GLEW_OK)
		{
			std::cerr << "Failed to initialise GLEW" << std::endl;
			exit(EXIT_FAILURE);
		}

		glfwSetKeyCallback(window, key_callback);

		// For Debugging Purposes
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(debug_message_callback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

		return window;
	}
};