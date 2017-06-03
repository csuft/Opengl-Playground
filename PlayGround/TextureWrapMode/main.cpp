#include <iostream>
#include <glew.h>  // should include before glfw3.h
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

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
	 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	 
	GLuint textureID = SOIL_load_OGL_texture("E:/OpenGL/Playground/PlayGround/TextureWrapMode/texture.jpg", 
		                                     SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
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
	 
	GLuint programID = LoadShaders("simple.vert", "simple.frag"/*, "simple.geom", true*/);
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
		glDrawElements(GL_LINE_STRIP, vert_indices.size(), GL_UNSIGNED_INT, (void*)0); // we use index buffer, so set it to null.  
		//glDrawArrays(GL_LINE_STRIP, 0, vert_coords.size());

		glDisableVertexAttribArray(1); 

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	glfwTerminate();
	glBindVertexArray(0);
	glDeleteBuffers(1, &vert_buffer);
	glDeleteBuffers(1, &indices_buffer); 
	glDeleteProgram(programID); 

	return 0;
}

void generateSphereGeometry(GLfloat radius, std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, std::vector<GLfloat>& texcoords)
{
	// 经度方向切割100段
	const GLuint VERTICAL_SLICE = 100;
	GLfloat vertical_step = (GLfloat)(glm::two_pi<GLfloat>() / VERTICAL_SLICE);
	// 纬度方向切割50段
	const GLuint HORIZONTAL_SLICE = 50;
	GLfloat horizontal_step = (GLfloat)(glm::pi<GLfloat>() / HORIZONTAL_SLICE);
	
	GLuint start_index = 0;
	GLuint current_index = 0;
	// 纬度方向上将球体分割成50段，即切割成50个不同半径的同心圆
	for (size_t i = 0; i <= HORIZONTAL_SLICE; ++i)
	{
		start_index = current_index;
		GLfloat vertical_angle = horizontal_step * i;
		GLfloat z_coord = radius * std::cos(vertical_angle);
		GLfloat sub_radius = radius * std::sin(vertical_angle);
		// 经度方向将球体切割成100段
		for (size_t j = 0; j <= VERTICAL_SLICE; j++)
		{
			GLfloat horizontal_angle = vertical_step * j;
			GLfloat x_coord = sub_radius * std::cos(horizontal_angle);
			GLfloat y_coord = sub_radius * std::sin(horizontal_angle);

			// 一圈结束了，起点和重点重合
			if (j == VERTICAL_SLICE)
			{
				vertices.push_back(vertices[start_index]);
				vertices.push_back(vertices[start_index + 1]);
				vertices.push_back(vertices[start_index + 2]);
			}
			else
			{
				// 上面在计算x, y, z坐标时将Y轴和Z轴调换了
				// 因此在这里要调整坐标系为右手坐标系
				vertices.push_back(x_coord);
				vertices.push_back(z_coord);
				vertices.push_back(y_coord);
			}
			// 保存每一圈的起始点索引，方便计算每一圈的重合点
			current_index += 3;

			if (i > 0 && j > 0)
			{
				// 相邻上一圈中的顶点索引
				GLuint bottom_ring_a = (VERTICAL_SLICE + 1)*i + j;
				GLuint bottom_ring_b = (VERTICAL_SLICE + 1)*i + j - 1;
				// 相邻下一圈中的顶点索引
				GLuint top_ring_a = (VERTICAL_SLICE + 1)*(i - 1) + j;
				GLuint top_ring_b = (VERTICAL_SLICE + 1)*(i - 1) + j - 1;

				// j == 1时，相邻上一圈收缩成一个点
				if (j == 1)
				{
					indices.push_back(bottom_ring_a);
					indices.push_back(top_ring_a);
					indices.push_back(top_ring_b);
				}
				// j == HORIZONTAL_SLICE时，相邻下一圈收缩成一个点
				else if (j == HORIZONTAL_SLICE)
				{
					indices.push_back(bottom_ring_a);
					indices.push_back(top_ring_b);
					indices.push_back(bottom_ring_b);
				}
				else
				{
					// 逆时钟方向连接顶点，每次连接成一个四边形
					// 逆时钟方向避免三角形被背面剔除处理掉
					indices.push_back(bottom_ring_a);
					indices.push_back(top_ring_a);
					indices.push_back(top_ring_b);
					indices.push_back(bottom_ring_a);
					indices.push_back(top_ring_b);
					indices.push_back(bottom_ring_b);
				}
			}
			texcoords.push_back((GLfloat)j/VERTICAL_SLICE);
			texcoords.push_back(1 - (GLfloat)i/HORIZONTAL_SLICE);
		}
	}
} 