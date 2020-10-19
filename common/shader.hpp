#ifndef SHADER_HPP
#define SHADER_HPP

#include <utility>
#include <vector>
#include <string>
#include <glad/glad.h>

using ShaderVector = std::vector<std::pair<std::string, GLenum>>;
GLuint LoadAllShaders(ShaderVector& shaders);

#endif
