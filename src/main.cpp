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

struct FrameData
{
	double lastUpdateTime = glfwGetTime();
	double elapsedTime = lastUpdateTime;
	double frameTime = 0.0f;
	int frameCount = 0;
	int FPS = 0;
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

static void InitTweakBar(const Window& t_Window, int& FPS, double& frameTime)
{
	TwBar* TweakBar;

	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(t_Window.getWidth(), t_Window.getHeight());
	TwDefine(" TW_HELP visible=false "); // disable help menu
	TwDefine(" GLOBAL fontsize=3 ");	 // set large font size

	TweakBar = TwNewBar("Main");
	TwDefine(" Main Label ='Controls' refresh=0.02 text=light size='220 250' ");

	TwAddVarRO(TweakBar, "FPS", TW_TYPE_INT32, &FPS, " group='Frame' ");
	TwAddVarRO(TweakBar, "Frame Time", TW_TYPE_DOUBLE, &frameTime, " group='Frame' precision=4 ");
}

static void drawGUI(FrameData& t_Frame)
{
	TwDraw();

	t_Frame.frameCount++;
	t_Frame.elapsedTime = glfwGetTime() - t_Frame.lastUpdateTime;

	if (t_Frame.elapsedTime >= 1.0f)
	{
		t_Frame.frameTime = 1.0f / t_Frame.frameCount;

		std::string str = "FPS = " + std::to_string(t_Frame.frameCount) + "; FT = " + std::to_string(t_Frame.frameTime);

		t_Frame.FPS = t_Frame.frameCount;
		t_Frame.frameCount = 0;
		t_Frame.lastUpdateTime += t_Frame.elapsedTime;
	}
}

int main()
{
	FrameData frame;

	Window window(1000, 800);
	InitTweakBar(window, frame.FPS, frame.frameTime);

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

		drawGUI(frame);

		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
	}
}
