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
GLuint texIds[3];  
const int WIDTH = 914;
const int HEIGHT = 558;

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

	window = glfwCreateWindow(1000, 1000, "PlayGround for OpenGL - YUV Shader", NULL, NULL);
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
	shaders.push_back(std::make_pair<std::string, GLenum>("7-shader.vert", GL_VERTEX_SHADER));
	shaders.push_back(std::make_pair<std::string, GLenum>("7-shader.frag", GL_FRAGMENT_SHADER)); 
	GLuint programID = LoadAllShaders(shaders);
	if (programID <= 0)
	{
		glfwTerminate();
		std::cout << "failed to load shaders." << std::endl;
		return -4;
	}
	// read raw frame data 
#ifdef YUV420P
	FILE* file = fopen("914x558.yuv420p", "rb");
#elif NV12
	FILE* file = fopen("914x558.nv12", "rb");
#elif NV21
	FILE* file = fopen("914x558.nv21", "rb");
#endif
	if (file == nullptr)
	{
		std::cout << "file pointer is null" << std::endl;
		glfwTerminate();
		return -5;
	}
	char* rawData = new char[WIDTH*HEIGHT * 2];
	fread(rawData, 1, WIDTH*HEIGHT * 2, file);
	fclose(file);

	// load all three planes as OpenGL texture.
	glGenTextures(3, texIds);
	glBindTexture(GL_TEXTURE_2D, texIds[0]); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, texIds[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, texIds[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	if (rawData)
	{
		delete[] rawData;
	} 

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);  
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glViewport(0, 0, 1000, 1000);

	GLuint mvpID = glGetUniformLocation(programID, "MVP");
	GLuint samplerYID = glGetUniformLocation(programID, "texSamplerY");
	GLuint samplerUID = glGetUniformLocation(programID, "texSamplerU");
	GLuint samplerVID = glGetUniformLocation(programID, "texSamplerV");

	glm::mat4 modelMat(1.0f);
	glm::mat4 viewMat;
	glm::mat4 projectionMat;
	glm::mat4 MVP;
	
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		 
		glUseProgram(programID);  
		
		// Projection matrix : 45?Field of View, 1:1 ratio, display range : 0.1 unit <-> 100 units
		projectionMat = glm::perspective(glm::radians(45.0f), 1.0f / 1.0f, 0.1f, 1000.0f);
		// Camera matrix
		viewMat = glm::lookAt(
			glm::vec3(0, 0, 2),  // Camera is here
			glm::vec3(0, 0, -1), // and looks here : at the same position, plus "direction"
			glm::vec3(0, 1, 0)   // Head is up (set to 0,-1,0 to look upside-down)
		);
		MVP = projectionMat * viewMat * modelMat;
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, glm::value_ptr(MVP));
		
		// bind texture 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texIds[0]); 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, WIDTH, HEIGHT, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, rawData);
		glUniform1i(samplerYID, 0);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texIds[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, WIDTH / 2, HEIGHT / 2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, rawData + WIDTH*HEIGHT);
		glUniform1i(samplerUID, 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texIds[2]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, WIDTH / 2, HEIGHT / 2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, rawData + WIDTH*HEIGHT * 5 / 4);
		glUniform1i(samplerVID, 2);

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
	glDeleteTextures(3, texIds);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID); 

	return 0;
} 