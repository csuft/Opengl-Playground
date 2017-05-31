#include <iostream>
#include <glew.h>  // should include before glfw3.h
#include <glfw3.h>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "shader.hpp"
#include "Sphere.h"
#include "controls.hpp"
GLFWwindow* window; 

void computeMatrix(glm::mat4& projection, glm::mat4& view);

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
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	window = glfwCreateWindow(1024, 768, "PlayGround for OpenGL", NULL, NULL);
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

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);
	 
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint programID = LoadShaders("sphereShader.vert", "sphereShader.frag"/*, "simple.geom", true*/);
	GLuint vertexPosistion_modelSpaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");

	Sphere* sphere = new Sphere;
	sphere->init(vertexPosistion_modelSpaceID);
	 
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glViewport(0, 0, 1024, 768);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
   
	do
	{
		glClear(GL_COLOR_BUFFER_BIT);

		// do something drawing.
		glUseProgram(programID);
		 
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MV = ViewMatrix * ModelMatrix;

		GLuint projectionMatID = glGetUniformLocation(programID, "projMatrix");
		GLuint modelViewMatID = glGetUniformLocation(programID, "mvMatrix");

		glUniformMatrix4fv(projectionMatID, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
		glUniformMatrix4fv(modelViewMatID, 1, GL_FALSE, glm::value_ptr(MV));

		sphere->draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	glfwTerminate();
	glBindVertexArray(0); 
	glDeleteProgram(programID); 

	sphere->cleanup();
	delete sphere;

	return 0;
}
 
void computeMatrix(glm::mat4& projection, glm::mat4& view)
{
	glm::vec3 orientation(0, 0, -1);
	glm::vec3 right(1, 0, 0);
	glm::vec3 up = glm::cross(orientation, up);
	glm::vec3 position(0.0f, 0.0f, 5.0f);

	projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	view = glm::lookAt(position, position + orientation, up);
}