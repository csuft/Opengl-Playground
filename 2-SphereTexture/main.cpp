#include <iostream>
#include <glad/glad.h>  // should include before glfw3.h
#include <glfw3.h>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
using namespace glm;

#include "shader.hpp"
#include "controls.hpp"
#include "models.hpp"
GLFWwindow* window;

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

	window = glfwCreateWindow(1024, 768, "PlayGround for OpenGL - Sphere Texture", NULL, NULL);
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	 
	GLuint textureID = SOIL_load_OGL_texture("texture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (textureID  == 0)
	{
		std::cout << "Failed to load OpenGL texture." << std::endl;
		glfwTerminate();
		return -4;
	}

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	std::vector<GLfloat> vert_coords;
	std::vector<unsigned int> vert_indices;
	std::vector<GLfloat> texcoords;
	generateSphereGeometry(2.0f, vert_coords, vert_indices, texcoords); 

	// Vertices
	GLuint vert_buffer;
	glGenBuffers(1, &vert_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
	glBufferData(GL_ARRAY_BUFFER, vert_coords.size()*sizeof(GLfloat), vert_coords.data(), GL_STATIC_DRAW);

	// Indices
	GLuint indices_buffer;
	glGenBuffers(1, &indices_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vert_indices.size()*sizeof(GLuint), vert_indices.data(), GL_STATIC_DRAW);

	// UVs
	GLuint uv_buffer;
	glGenBuffers(1, &uv_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glBufferData(GL_ARRAY_BUFFER, texcoords.size()*sizeof(GLfloat), texcoords.data(), GL_STATIC_DRAW);
	 
	ShaderVector shaders;
	shaders.push_back(std::make_pair<std::string, GLenum>("2-SphereTexture.vert", GL_VERTEX_SHADER));
	shaders.push_back(std::make_pair<std::string, GLenum>("2-SphereTexture.frag", GL_FRAGMENT_SHADER));
	GLuint programID = LoadAllShaders(shaders);
	if (programID == 0)
	{
		glfwTerminate();
		return -5;
	}

	GLuint mvpID = glGetUniformLocation(programID, "MVP");
	GLuint samplerID = glGetUniformLocation(programID, "texSampler");
	 
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glViewport(0, 0, 1024, 768);
	
	glm::mat4 modelMat(1.0f);
	glm::mat4 viewMat;
	glm::mat4 projectionMat;
	glm::mat4 MVP;
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		 
		glUseProgram(programID);  
		
		computeMatricesFromInputs();
		viewMat = getViewMatrix();
		projectionMat = getProjectionMatrix();
		MVP = projectionMat * viewMat * modelMat;
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, glm::value_ptr(MVP));

		// bind texture unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glUniform1i(samplerID, 0);

		// upload vertices
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);  

		// upload uv coordinates
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// use indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
		glDrawElements(GL_TRIANGLES, vert_indices.size(), GL_UNSIGNED_INT, (void*)0); // we use index buffer, so set it to null.  
		//glDrawArrays(GL_LINE_STRIP, 0, vert_coords.size());

		glDisableVertexAttribArray(1); 

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	glfwTerminate();
	glBindVertexArray(0);
	glDeleteBuffers(1, &vert_buffer);
	glDeleteBuffers(1, &indices_buffer); 
	glDeleteBuffers(1, &uv_buffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID); 

	return 0;
}

