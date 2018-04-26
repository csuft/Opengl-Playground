#include <iostream>
#include <glew.h>  // should include before glfw3.h
#include <glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>

#include "models.hpp"
#include "shader.hpp" 

GLFWwindow* window; 
GLuint texIds;  

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

	window = glfwCreateWindow(1024, 768, "PlayGround for OpenGL - Plane Texture", NULL, NULL);
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
 
	std::vector<GLfloat> vert_coords;
	std::vector<unsigned int> vert_indices;
	std::vector<GLfloat> texcoords;
	generatePlaneGeometry(vert_coords, vert_indices, texcoords); 

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
	shaders.push_back(std::make_pair<std::string, GLenum>("6-plane-projection.vert", GL_VERTEX_SHADER));
	shaders.push_back(std::make_pair<std::string, GLenum>("6-plane-projection.frag", GL_FRAGMENT_SHADER)); 
	GLuint programID = LoadAllShaders(shaders);
	if (programID == 0)
	{
		glfwTerminate();
		return -4;
	}  

	texIds = SOIL_load_OGL_texture("input.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texIds == 0)
	{
		std::cout << "Failed to load OpenGL texture." << std::endl;
		glfwTerminate();
		return -4;
	}
	  
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);  
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glViewport(30, 30, 800, 600);

	GLuint mvpID = glGetUniformLocation(programID, "MVP");
	GLuint samplerID = glGetUniformLocation(programID, "texSampler");

	glm::mat4 modelMat(1.0f);
	glm::mat4 viewMat;
	glm::mat4 projectionMat;
	glm::mat4 MVP;
	
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		 
		glUseProgram(programID);  
		
		//// Projection matrix : 45?Field of View, 1:1 ratio, display range : 0.1 unit <-> 100 units
		//projectionMat = glm::perspective(glm::radians(45.0f), 1.0f / 1.0f, 0.1f, 1000.0f);
		//// Camera matrix
		//viewMat = glm::lookAt(
		//	glm::vec3(0, 0, 2),  // Camera is here
		//	glm::vec3(0, 0, -1), // and looks here : at the same position, plus "direction"
		//	glm::vec3(0, 1, 0)   // Head is up (set to 0,-1,0 to look upside-down)
		//);
		//MVP = projectionMat * viewMat * modelMat;
		//glUniformMatrix4fv(mvpID, 1, GL_FALSE, glm::value_ptr(MVP));
		
		// bind texture unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texIds);
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

		glDisableVertexAttribArray(1); 

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	glfwTerminate(); 
	glDeleteBuffers(1, &vert_buffer);
	glDeleteBuffers(1, &indices_buffer); 
	glDeleteBuffers(1, &uv_buffer);
	glDeleteTextures(1, &texIds);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID); 

	return 0;
} 