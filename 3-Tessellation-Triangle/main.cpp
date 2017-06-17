#include <iostream>
#include <glew.h>  // should include before glfw3.h
#include <glfw3.h>
#include <vector>
#include "shader.hpp" 
GLFWwindow* window;

void generateSphereGeometry(GLfloat radius, std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, std::vector<GLfloat>& texcoords);

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

	window = glfwCreateWindow(1024, 768, "PlayGround for OpenGL - Tessellation Triangle", NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "Failed to create window for OpenGL." << std::endl;
		glfwTerminate();
		getchar();
		return -2;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize glew." << std::endl;
		glfwTerminate();
		getchar();
		return -3;
	}
	 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
 
	ShaderVector shaders;
	shaders.push_back(std::make_pair<std::string, GLenum>("3-tessellation-triangle.vert", GL_VERTEX_SHADER));
	shaders.push_back(std::make_pair<std::string, GLenum>("3-tessellation-triangle.frag", GL_FRAGMENT_SHADER));
	shaders.push_back(std::make_pair<std::string, GLenum>("3-tessellation-triangle.tcs", GL_TESS_CONTROL_SHADER));
	shaders.push_back(std::make_pair<std::string, GLenum>("3-tessellation-triangle.tes", GL_TESS_EVALUATION_SHADER));
	GLuint programID = LoadAllShaders(shaders);
	if (programID == 0)
	{
		glfwTerminate();
		return -4;
	} 
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glViewport(0, 0, 1024, 768);
	
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		 
		glUseProgram(programID);  

		glDrawArrays(GL_PATCHES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	glfwTerminate(); 
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID); 

	return 0;
}