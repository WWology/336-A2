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
	GLfloat colours[3];
	// GLfloat normals[3];
};

struct Mesh
{
	Vertex* pMeshVertices = nullptr;
	GLuint numberOfVertices = 0;
	// GLuint* pMeshIndices = nullptr;
	// GLuint numberOfFaces;
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

static bool load_mesh(const char* t_FileName, Mesh* t_Mesh)
{
	const aiScene* pScene = aiImportFile(t_FileName, aiProcess_Triangulate);

	if (!pScene)
	{
		std::cerr << "Failed to load mesh" << std::endl;
		exit(EXIT_FAILURE);
	}

	const aiMesh* pMesh = pScene->mMeshes[0];

	t_Mesh->numberOfVertices = pMesh->mNumVertices;

	if (pMesh->HasPositions())
	{
		t_Mesh->pMeshVertices = new Vertex[pMesh->mNumVertices];

		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			const aiVector3D* pVertexPos = &(pMesh->mVertices[i]);

			t_Mesh->pMeshVertices[i].positions[0] = (GLfloat)pVertexPos->x;
			t_Mesh->pMeshVertices[i].positions[1] = (GLfloat)pVertexPos->y;
			t_Mesh->pMeshVertices[i].positions[2] = (GLfloat)pVertexPos->z;

			t_Mesh->pMeshVertices[i].colours[0] = static_cast<double>(rand()) / RAND_MAX;
			t_Mesh->pMeshVertices[i].colours[1] = static_cast<double>(rand()) / RAND_MAX;
			t_Mesh->pMeshVertices[i].colours[2] = static_cast<double>(rand()) / RAND_MAX;
		}
	}

	// if (pMesh->HasNormals())
	// {
	// 	for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
	// 	{
	// 		const aiVector3D* pVertexNormal = &(pMesh->mNormals[i]);

	// 		t_Mesh->pMeshVertices[i].normals[0] = (GLfloat)pVertexNormal->x;
	// 		t_Mesh->pMeshVertices[i].normals[1] = (GLfloat)pVertexNormal->y;
	// 		t_Mesh->pMeshVertices[i].normals[2] = (GLfloat)pVertexNormal->z;
	// 	}
	// }

	// if (pMesh->HasFaces())
	// {
	// 	t_Mesh->numberOfFaces = pMesh->mNumFaces;
	// 	t_Mesh->pMeshIndices = new GLuint[pMesh->mNumFaces * 3];

	// 	for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
	// 	{
	// 		const aiFace* pFace = &(pMesh->mFaces[i]);

	// 		t_Mesh->pMeshIndices[i * 3] = (GLuint)pFace->mIndices[0];
	// 		t_Mesh->pMeshIndices[i * 3 + 1] = (GLuint)pFace->mIndices[1];
	// 		t_Mesh->pMeshIndices[i * 3 + 2] = (GLuint)pFace->mIndices[2];
	// 	}
	// }

	aiReleaseImport(pScene);

	return true;
}

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
	Mesh mesh;
	gui.lightPos = light.position;

	load_mesh("res/models/torus.obj", &mesh);

	Window window(1000, 800);
	InitTweakBar(window, gui);

	glm::mat4 g_viewMatrix;
	glm::mat4 g_projectionMatrix;

	Shader shader("res/shaders/vShader.vert", "res/shaders/fShader.frag");
	shader.Bind();

	GLuint colourIdx = glGetAttribLocation(shader.getID(), "aColour");
	GLuint positionIdx = glGetAttribLocation(shader.getID(), "aPosition");
	shader.getUniformLocation("uModelMatrix");

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	g_viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	int width, height;
	glfwGetFramebufferSize(window.getWindow(), &width, &height);
	float aspectRatio = static_cast<float>(width) / height;
	g_projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

	VertexBuffer vbo(mesh.pMeshVertices, sizeof(Vertex) * mesh.numberOfVertices);
	// IndexBuffer ibo(mesh.pMeshIndices, sizeof(GLuint) * 3 * mesh.numberOfFaces);

	VertexArray vao;
	vao.Bind();
	vao.addBuffer(vbo, {positionIdx, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, positions))});
	vao.addBuffer(vbo, {colourIdx, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, colours))});

	Renderer renderer;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	while (!glfwWindowShouldClose(window.getWindow()))
	{
		renderer.Clear();
		glm::mat4 MVP = g_projectionMatrix * g_viewMatrix * modelMatrix;

		shader.setUniformMatrix4fv("uModelMatrix", &MVP[0][0]);

		renderer.Draw(vao, shader);

		drawGUI(gui);

		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
	}

	TwTerminate();
}
