#include <iostream>
#include <glad/glad.h>  // should include before glfw3.h
#include <glfw3.h>

#include <glm/glm.hpp>
using namespace glm;

#include "shader.hpp"
GLFWwindow* window;

void windowFreshCallback(GLFWwindow*);
void windowSizeCallback(GLFWwindow*, int, int);
void windowCloseCallback(GLFWwindow*);
void windowFocusCallback(GLFWwindow*, int);
void windowPosCallback(GLFWwindow*, int, int); 

int main(void)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		std::cout << "Failed to initialize glfw." << std::endl;
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1024, 768, "PlayGround for OpenGL - Startup", NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "Failed to create window for OpenGL." << std::endl;
		glfwTerminate();
		getchar();
		return -2;
	}
	
	glfwMakeContextCurrent(window);
	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize glad." << std::endl;
		glfwTerminate();
		getchar();
		return -3;
	}

	// set callbacks;
	glfwSetWindowPosCallback(window, windowPosCallback);
	glfwSetWindowRefreshCallback(window, windowFreshCallback);
	glfwSetWindowFocusCallback(window, windowFocusCallback);
	glfwSetWindowCloseCallback(window, windowCloseCallback);
	glfwSetWindowSizeCallback(window, windowSizeCallback);

	glViewport(0, 0, 600, 400);
	// Prepare data for drawing.
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	static const GLfloat vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	ShaderVector shaders;
	shaders.push_back(std::make_pair<std::string, GLenum>("1-Startup.vert", GL_VERTEX_SHADER));
	shaders.push_back(std::make_pair<std::string, GLenum>("1-Startup.frag", GL_FRAGMENT_SHADER));

	GLuint programID = LoadAllShaders(shaders);
	if (programID == 0)
	{
		glfwTerminate();
		return -4;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.0, 0.0, 0.4, 0.0);

	do 
	{
		glClear(GL_COLOR_BUFFER_BIT);
		
		// do something drawing.
		glUseProgram(programID);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		     glfwWindowShouldClose(window) == 0);

	glfwTerminate();
	getchar(); 

	return 0;
}

void windowFreshCallback(GLFWwindow* window)
{
	std::cout << "Window Refreshed." << std::endl;
}

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
	std::cout << "Window Resized." << std::endl;
	std::cout << "Width:" << width << "Height:" << height;
}

void windowCloseCallback(GLFWwindow* window)
{
	std::cout << "Window Closed." << std::endl;
}

void windowFocusCallback(GLFWwindow* window, int getFocus)
{
	std::cout << "Windows Focused." << std::endl;
	std::cout << "Focus state:" << getFocus << std::endl;
}

void windowPosCallback(GLFWwindow* window, int xPos, int yPos)
{
	std::cout << "Window Position." << std::endl;
	std::cout << "X position:" << xPos << "Y position:" << yPos << std::endl;
}