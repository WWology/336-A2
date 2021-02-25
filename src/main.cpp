// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>

// OpenGL Libraries
#include <AntTweakBar.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// My Own Files
#include "Callbacks.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Window.h"

struct Light
{
	glm::vec3 position = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 direction = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
};

struct FrameData
{
	double lastUpdateTime = glfwGetTime();
	double elapsedTime = lastUpdateTime;
	double frameTime = 0.0f;
	int frameCount = 0;
	int FPS = 0;
};

struct GUI
{
	FrameData frame;
	bool animation;
	glm::vec3 lightPos;
	float yaw = 0;
	float pitch = 0;
};

struct Vertex
{
	GLfloat positions[3];
	GLfloat colours[3];
};

// TODO Add texture
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

static void InitTweakBar(const Window& t_Window, GUI& t_GUI)
{
	TwBar* TweakBar;

	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(t_Window.getWidth(), t_Window.getHeight());
	TwDefine(" TW_HELP visible=false "); // disable help menu
	TwDefine(" GLOBAL fontsize=3 ");	 // set large font size

	TweakBar = TwNewBar("Main");
	TwDefine(" Main Label ='Controls' refresh=0.02 text=light size='220 250' ");

	TwAddVarRO(TweakBar, "FPS", TW_TYPE_INT32, &t_GUI.frame.FPS, " group='Frame' ");
	TwAddVarRO(TweakBar, "Frame Time", TW_TYPE_DOUBLE, &t_GUI.frame.frameTime, " group='Frame' precision=4 ");

	TwAddVarRW(TweakBar, "Toggle", TW_TYPE_BOOLCPP, &t_GUI.animation, " group='Animation'");

	TwAddVarRW(TweakBar, "Position x", TW_TYPE_FLOAT, &t_GUI.lightPos[0], " group='Light' min=-10.0 max=10.0 step=0.1");
	TwAddVarRW(TweakBar, "Position y", TW_TYPE_FLOAT, &t_GUI.lightPos[1], " group='Light' min=-10.0 max=10.0 step=0.1");
	TwAddVarRW(TweakBar, "Position z", TW_TYPE_FLOAT, &t_GUI.lightPos[2], " group='Light' min=0.0 max=10.0 step=0.1");

	TwAddVarRW(TweakBar, "Yaw", TW_TYPE_FLOAT, &t_GUI.yaw, " group='Camera' min=0.0 max=10.0 step=0.1");
	TwAddVarRW(TweakBar, "Pitch", TW_TYPE_FLOAT, &t_GUI.pitch, " group='Camera' min=0.0 max=10.0 step=0.1");
}

static void drawGUI(GUI& t_GUI)
{
	TwDraw();

	t_GUI.frame.frameCount++;
	t_GUI.frame.elapsedTime = glfwGetTime() - t_GUI.frame.lastUpdateTime;

	if (t_GUI.frame.elapsedTime >= 1.0f)
	{
		t_GUI.frame.frameTime = 1.0f / t_GUI.frame.frameCount;

		t_GUI.frame.FPS = t_GUI.frame.frameCount;
		t_GUI.frame.frameCount = 0;
		t_GUI.frame.lastUpdateTime += t_GUI.frame.elapsedTime;
	}
}

int main()
{
	GUI gui;
	Light light;
	gui.lightPos = light.position;

	Window window(1000, 800);
	InitTweakBar(window, gui);

	Shader shader("res/shaders/vShader.vert", "res/shaders/fShader.frag");
	shader.Bind();

	GLuint colourIdx = glGetAttribLocation(shader.getID(), "aColour");
	GLuint positionIdx = glGetAttribLocation(shader.getID(), "aPosition");
	shader.getUniformLocation("uModelMatrix");

	glm::mat4 modelMatrix = glm::mat4(1.0f);

	VertexBuffer vbo(vertices, sizeof(vertices));
	IndexBuffer ibo(indices, sizeof(indices));

	VertexArray vao;
	vao.Bind();
	vao.addBuffer(vbo, {positionIdx, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, positions))});
	vao.addBuffer(vbo, {colourIdx, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, colours))});

	Renderer renderer;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	while (!glfwWindowShouldClose(window.getWindow()))
	{
		renderer.Clear();
		shader.setUniformMatrix4fv("uModelMatrix", &modelMatrix[0][0]);

		renderer.Draw(vao, ibo, shader);

		drawGUI(gui);

		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
	}

	TwTerminate();
}
