// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>

// OpenGL Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// My Own Files
#include "Callbacks.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

// Constant Variable
static const uint32_t WINDOW_WIDTH = 800;
static const uint32_t WINDOW_HEIGHT = 800;

struct Vertex
{
	GLfloat positions[3];
	GLfloat colours[3];
};

Vertex vertices[] = {
	// Triangle
	// vertex 1
	-0.5f, 0.5f, 0.0f, // position
	1.0f, 0.0f, 0.0f,  // colour
	//vertex 2
	-0.5f, -0.5f, 0.0f, // position
	1.0f, 0.5f, 0.0f,	// colour
	// vertex 3
	0.5f, 0.5f, 0.0f, // position
	1.0f, 0.0f, 0.5f, // colour
	// Vertex 2
	0.5f, -0.5f, 0.0f, // position
	0.7f, 1.0f, 0.2f,  // colour
};

GLuint indices[] = {
	0, 1, 2, // Triangle 1
	2, 1, 3	 // Triangle 2
};

static void init(GLuint iboID)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
}

static void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glFlush();
}

int main()
{
	GLFWwindow* window = nullptr;

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

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "5281568_YosuaMartiansia_A2", nullptr, nullptr);

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

	Shader shader("../res/shaders/vShader.vert", "../res/shaders/fShader.frag");
	shader.Bind();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLuint colourIdx = glGetAttribLocation(shader.getID(), "aColour");
	GLuint positionIdx = glGetAttribLocation(shader.getID(), "aPosition");

	VertexBuffer vbo(vertices, sizeof(vertices));
	IndexBuffer ibo(indices, sizeof(indices));
	ibo.Bind();

	VertexArray vao;
	vao.Bind();
	vao.addBuffer(vbo, {positionIdx, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, positions))});
	vao.addBuffer(vbo, {colourIdx, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, colours))});

	init(ibo.getID());

	while (!glfwWindowShouldClose(window))
	{
		render_scene();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
