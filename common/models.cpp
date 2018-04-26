#include "models.hpp"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

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
				// 上面在计算x, y, z坐标时坐标系+Z轴向上，+X轴指向屏幕外
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
				 
				// 逆时钟方向避免三角形被背面剔除处理掉
				indices.push_back(bottom_ring_a);
				indices.push_back(top_ring_a);
				indices.push_back(top_ring_b);
				indices.push_back(bottom_ring_a);
				indices.push_back(top_ring_b);
				indices.push_back(bottom_ring_b);
				
			} 

			texcoords.push_back((GLfloat)j/VERTICAL_SLICE);
			texcoords.push_back(1 - (GLfloat)i/HORIZONTAL_SLICE);
		}
	}
} 

void generatePlaneGeometry(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, std::vector<GLfloat>& texcoords)
{
	GLfloat vert[] = {
		0.0f,  0.0f, 0.0f,     
		1.0f,  0.0f, 0.0f,   
		1.0f, 1.0f, 0.0f,     
		0.0f,  1.0f, 0.0f,     
	};
	for (int i = 0; i < 12; i++)
	{
		vertices.push_back(vert[i]);
	}

	GLfloat texs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	for (int i = 0; i < 8; ++i)
	{
		texcoords.push_back(texs[i]);
	}

	GLuint inds[] = {
		0, 1, 2,
		0, 2, 3
	};
	for (int i = 0; i < 6; i++)
	{
		indices.push_back(inds[i]);
	}
}