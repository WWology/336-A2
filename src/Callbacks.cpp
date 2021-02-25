// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>

#include <AntTweakBar.h>

#include "Callbacks.h"

/**
 * @brief Function pointer for OpenGL Error (Debug only)
 */
void debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam)
{
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:
			std::cerr << message << " (HIGH)" << std::endl;
			return;
		case GL_DEBUG_SEVERITY_MEDIUM:
			std::cerr << message << " (MEDIUM)" << std::endl;
			return;
		case GL_DEBUG_SEVERITY_LOW:
			std::cerr << message << " (LOW)" << std::endl;
			return;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			std::cerr << message << " (NOTIFY)" << std::endl;
			return;
	}
	std::cerr << "Source: " << source << ", Type: " << type << ", ID: " << id
			  << ", Severity: " << severity << ", Length: " << length << ", Message: " << message << std::endl;
}

/**
 * @brief Function used for managing keyboard & mouse input for OpenGL
 *
 * @param window The window context where the input is happening
 * @param key The Key pressed
 * @param action Type of action performed
 */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

/**
 * @brief Function used to manage errors in GLFW
 *
 * @param description The string containing the error message
 */
void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl;
}

/**
 * @brief For modifying AntTweakBar GUI
 *
 * @param window The window where the gui is to be modified
 * @param xpos x position of the cursor in the window
 * @param ypos y position of the cursor in the window
 */
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// pass mouse data to tweak bar
	TwEventMousePosGLFW(xpos, ypos);
}

/**
 * @brief For modifying AntTweakBar GUI
 *
 * @param window The window where the gui is to be modified
 * @param button The button pressed on the mouse
 * @param action The action performed on the button
 */
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// pass mouse data to tweak bar
	TwEventMouseButtonGLFW(button, action);
}