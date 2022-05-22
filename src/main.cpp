#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Filesystem.h"
#include "Camera.h"
#include "generateSphere.h"
#include "sphereProperty.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <string>

void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void initData();
void initTexture();

const int windowWidth = 800;
const int windowHeight = 600;

unsigned int VAO;
unsigned int texture;

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool  firstMouse = true;

//time
float deltatime = 0.0f;
float lastframe = 0.0f;

//鼠标移动的灵敏度
float sensitivity = 4.0f;

//sphere
#define OSSphereSliceNum  300
#define OSSphereRadius    1.0   // 球体模型半径
GLint	_numIndices;             //索引个数
int _numPoints;
GLuint	program;

bool isLineMode = false;

int main(int argc, char* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", NULL, NULL);
	if (NULL == window)
	{
		std::cout << "glfw create window failed!" << std::endl;
		glfwTerminate();
		
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//lamda
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height){

		glViewport(0, 0, width, height);
	});

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	Shader shader(FileSystem::getPath("resources/shader/vertexShader").c_str(),FileSystem::getPath("resources/shader/fragmentShader").c_str());

	//initData();
	//SphereProperty sphereProperty(&VAO, &_numPoints, OSSphereRadius);
	SphereProperty sphereProperty(&VAO, &_numIndices, OSSphereRadius, 100, 200);
	initTexture();

	shader.use(); //use之后才能 set
	
	shader.setInt("ourTexture", 0);
	int modelHandle =  glGetUniformLocation(shader.program, "model");
	int viewHandle = glGetUniformLocation(shader.program, "view");
	int projectionHandle = glGetUniformLocation(shader.program, "project");


	while (!glfwWindowShouldClose(window))
	{	
		float currentTime = glfwGetTime();
		deltatime = currentTime - lastframe;
		lastframe = currentTime;

		processInput(window);

		//Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);


		//Model View Projection
		glm::mat4 projections;
		projections = glm::perspective(glm::radians(camera.mZoom), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
		//shader.setMatrix4("project", projections); //每帧调用glGetUniformLocation会导致cpu急剧上升，在初始化的时候获取uniform的handle，可以减少CPU消耗
		glUniformMatrix4fv(projectionHandle, 1, GL_FALSE, &projections[0][0]);

		glm::mat4 mods(1.0f);
		//shader.setMatrix4("model", mods);
		glUniformMatrix4fv(modelHandle, 1, GL_FALSE, &mods[0][0]);

		glm::mat4 views = camera.GetViewMatrix();
		//shader.setMatrix4("view", views);
		glUniformMatrix4fv(viewHandle, 1, GL_FALSE, &views[0][0]);

		glDrawElements(GL_TRIANGLES, _numIndices, GL_UNSIGNED_SHORT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, _numPoints);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();

	return 0;
}

void initData()
{
	//set vertex data
	GLuint _vertexBuffer;
	GLuint _textureCoordBuffer;
	GLuint _indexBuffer;

	GLfloat  *_vertices;
	GLfloat  *_texCoords;
	GLushort *_indices;

	int numVertices = 0;  // 顶点的个数
	int strideNum = 3;    

	_numIndices = generateSphere(OSSphereSliceNum, OSSphereRadius, &_vertices, &_texCoords, &_indices, &numVertices);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// 加载顶点坐标 position
	glGenBuffers(1, &_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVertices*strideNum*sizeof(GLfloat), _vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numIndices*sizeof(GLushort), _indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, strideNum, GL_FLOAT, GL_FALSE, strideNum*sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);

	//加载纹理坐标 UV
	glGenBuffers(1, &_textureCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _textureCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * numVertices, _texCoords, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(1);

	//texture1
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	//wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load
	int imgWidth, imgHeight, imgChannel;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pData = stbi_load(FileSystem::getPath("resources/city.jpg").c_str(), &imgWidth, &imgHeight, &imgChannel, 0);
	if (pData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "load image1 failed." << std::endl;
	}
	stbi_image_free(pData);

	// 释放内存
	free(_vertices);
	free(_indices);
	free(_texCoords);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void initTexture()
{
	//texture1
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load
	int imgWidth, imgHeight, imgChannel;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pData = stbi_load(FileSystem::getPath("resources/city.jpg").c_str(), &imgWidth, &imgHeight, &imgChannel, 0);
	if (pData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "load image1 failed." << std::endl;
	}
	stbi_image_free(pData);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltatime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltatime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltatime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltatime);

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		isLineMode = !isLineMode;
		if (isLineMode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = (xpos - lastX) * sensitivity;
		float yoffset = (lastY - ypos) * sensitivity;

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}

	if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		lastX = xpos;
		lastY = ypos;
	}
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
		