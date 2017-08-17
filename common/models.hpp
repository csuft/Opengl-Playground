#ifndef __MODELS_HPP
#define __MODELS_HPP

#include <glew.h>
#include <vector>

void generateSphereGeometry(GLfloat radius, std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, std::vector<GLfloat>& texcoords);
void generatePlaneGeometry(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, std::vector<GLfloat>& texcoords);

#endif