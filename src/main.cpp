// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>

// OpenGL Libraries
#include <AntTweakBar.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
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
	glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
};

struct Material
{
	glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
	float shininess = 0;
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
	bool animation = true;
	glm::vec3 lightPos = glm::vec3(1.0f, 1.0f, 1.0f);
	float yaw = 0;
	float pitch = 0;
};

struct Vertex
{
	GLfloat positions[3];
	// GLfloat colours[3];
	GLfloat normals[3];
};

struct Mesh
{
	Vertex* pMeshVertices = nullptr;
	GLuint numberOfVertices = 0;
	GLuint* pMeshIndices = nullptr;
	GLuint numberOfFaces = 0;

	Mesh(const char* t_FileName)
	{
		const aiScene* pScene = aiImportFile(t_FileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

		if (!pScene)
		{
			std::cerr << "Failed to load mesh" << std::endl;
			exit(EXIT_FAILURE);
		}

		const aiMesh* pMesh = pScene->mMeshes[0];

		numberOfVertices = pMesh->mNumVertices;

		if (pMesh->HasPositions())
		{
			pMeshVertices = new Vertex[pMesh->mNumVertices];

			for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
			{
				const aiVector3D* pVertexPos = &(pMesh->mVertices[i]);

				pMeshVertices[i].positions[0] = (GLfloat)pVertexPos->x;
				pMeshVertices[i].positions[1] = (GLfloat)pVertexPos->y;
				pMeshVertices[i].positions[2] = (GLfloat)pVertexPos->z;
			}
		}

		if (pMesh->HasNormals())
		{
			for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
			{
				const aiVector3D* pVertexNormal = &(pMesh->mNormals[i]);

				pMeshVertices[i].normals[0] = (GLfloat)pVertexNormal->x;
				pMeshVertices[i].normals[1] = (GLfloat)pVertexNormal->y;
				pMeshVertices[i].normals[2] = (GLfloat)pVertexNormal->z;
			}
		}

		if (pMesh->HasFaces())
		{
			numberOfFaces = pMesh->mNumFaces;
			pMeshIndices = new GLuint[pMesh->mNumFaces * 3];

			for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
			{
				const aiFace* pFace = &(pMesh->mFaces[i]);

				pMeshIndices[i * 3] = (GLuint)pFace->mIndices[0];
				pMeshIndices[i * 3 + 1] = (GLuint)pFace->mIndices[1];
				pMeshIndices[i * 3 + 2] = (GLuint)pFace->mIndices[2];
			}
		}

		aiReleaseImport(pScene);
	}
};

GLfloat viewport_Lines[] = {
	// vertex 1
	0.0f, 600.0f, 0.0f, // position
	//vertex 2
	1600.0f, 600.0f, 0.0f, // position
	// vertex 3
	800.0f, 0.0f, 0.0f, // position
	// Vertex 4
	800.0f, 1200.0f, 0.0f, // position
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
	Material material;
	Mesh mesh("res/models/torus.obj");
	gui.lightPos = light.position;
	float zRotateAngle = 0.0f;

	Window window(1600, 1200);
	InitTweakBar(window, gui);

	Shader shader("res/shaders/vShader.vert", "res/shaders/fShader.frag");
	GLuint positionIdx = glGetAttribLocation(shader.getID(), "aPosition");
	GLuint normalIdx = glGetAttribLocation(shader.getID(), "aNormal");

	int width, height;
	glfwGetFramebufferSize(window.getWindow(), &width, &height);
	float aspectRatio = static_cast<float>(width) / height;

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
	glm::mat4 lineProjectionMatrix = glm::ortho(0.0f, 1600.0f, 0.0f, 1200.0f, 0.1f, 100.0f);

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	light.position = glm::vec3(10.0f, 10.0f, 10.0f);
	light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	light.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light.specular = glm::vec3(1.0f, 1.0f, 1.0f);

	material.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	material.diffuse = glm::vec3(0.2f, 0.7f, 1.0f);
	material.specular = glm::vec3(0.2f, 0.7f, 1.0f);
	material.shininess = 10.0f;

	VertexBuffer vbo(mesh.pMeshVertices, sizeof(Vertex) * mesh.numberOfVertices);
	IndexBuffer ibo(mesh.pMeshIndices, 3 * mesh.numberOfFaces);

	VertexBuffer vLines(&viewport_Lines, sizeof(viewport_Lines));
	VertexArray lines;
	lines.Bind();
	lines.addBuffer(vLines, {positionIdx, sizeof(GLfloat), nullptr});

	VertexArray vao;
	vao.Bind();
	vao.addBuffer(vbo, {positionIdx, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, positions))});
	vao.addBuffer(vbo, {normalIdx, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normals))});

	Renderer renderer;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	while (!glfwWindowShouldClose(window.getWindow()))
	{
		renderer.Clear();
		modelMatrix = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::radians(zRotateAngle), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));

		glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;
		glm::mat4 MV = viewMatrix * modelMatrix;

		shader.setUniformMatrix4fv("uMVPMatrix", &MVP[0][0]);
		shader.setUniformMatrix4fv("uMVMatrix", &MV[0][0]);
		shader.setUniformMatrix4fv("uVMatrix", &viewMatrix[0][0]);

		shader.setUniform3fv("uLight.position", &light.position[0]);
		shader.setUniform3fv("uLight.ambient", &light.ambient[0]);
		shader.setUniform3fv("uLight.diffuse", &light.diffuse[0]);
		shader.setUniform3fv("uLight.specular", &light.specular[0]);

		shader.setUniform3fv("uMaterial.ambient", &material.ambient[0]);
		shader.setUniform3fv("uMaterial.diffuse", &material.diffuse[0]);
		shader.setUniform3fv("uMaterial.specular", &material.specular[0]);
		shader.setUniform1fv("uMaterial.shininess", &material.shininess);

		glViewport(0, 0, 800, 600);
		drawGUI(gui);

		glViewport(800, 600, 800, 600);
		renderer.Draw(vao, ibo, shader);

		glViewport(0, 0, 800, 600);
		renderer.Draw(vao, ibo, shader);

		glViewport(800, 0, 800, 600);
		renderer.Draw(vao, ibo, shader);

		if (gui.animation)
		{
			zRotateAngle += 0.5f;
		}

		MVP = lineProjectionMatrix * viewMatrix;
		shader.setUniformMatrix4fv("uMVPMatrix", &MVP[0][0]);
		lines.Bind();
		glViewport(0, 0, 1600, 1200);
		glDrawArrays(GL_LINES, 0, 4);

		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
	}

	TwTerminate();
}
