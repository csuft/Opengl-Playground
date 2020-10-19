#include "shader.hpp"

#include <iostream>
#include <fstream>

// vs: vertex shader
GLuint LoadAllShaders(ShaderVector& shaders)
{ 
	GLint Result = GL_FALSE;
	GLuint shaderId;
	int InfoLogLength;
	
	GLuint programID = glCreateProgram();
	std::vector<GLuint> shaderIds;

	ShaderVector::iterator begin;
	for (begin = shaders.begin(); begin != shaders.end(); ++begin)
	{
		std::string shaderFile = begin->first;
		GLenum shaderType = begin->second;
		switch(shaderType)
		{
			case GL_VERTEX_SHADER:
				shaderId = glCreateShader(GL_VERTEX_SHADER);
				break;
			case GL_FRAGMENT_SHADER:
				shaderId = glCreateShader(GL_FRAGMENT_SHADER);
				break;
			case GL_GEOMETRY_SHADER:
				shaderId = glCreateShader(GL_GEOMETRY_SHADER);
				break;
			case GL_TESS_CONTROL_SHADER:
				shaderId = glCreateShader(GL_TESS_CONTROL_SHADER);
				break;
			case GL_TESS_EVALUATION_SHADER:
				shaderId = glCreateShader(GL_TESS_EVALUATION_SHADER);
				break;
			default:
				break;
		}
		shaderIds.push_back(shaderId);
		
		std::string shaderCode;
		std::ifstream shaderFileStream(shaderFile, std::ios::in);
		if(shaderFileStream.is_open())
		{
			std::string Line = "";
			while(getline(shaderFileStream, Line))
				shaderCode += "\n" + Line;
			shaderFileStream.close();
		}
		else
		{
			std::cout << "Impossible to open " << shaderFile << ". Are you in the right directory ? Don't forget to read the FAQ !\n" << std::endl; 
			return -1;
		}
		
		std::cout << "Compiling shader... " << std::endl;
		char const * shaderSourcePointer = shaderCode.c_str();
		glShaderSource(shaderId, 1, &shaderSourcePointer , NULL);
		glCompileShader(shaderId);

		// Check compilation status
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if ( InfoLogLength > 0 ){
			GLchar* VertexShaderErrorMessage = new GLchar(InfoLogLength+1);
			glGetShaderInfoLog(shaderId, InfoLogLength, NULL, VertexShaderErrorMessage);
			std::cout << VertexShaderErrorMessage << std::endl;
			return -1;
		}
		 
		glAttachShader(programID, shaderId); 
	}
	
	// Link the program
	std::cout << "Linking program..." << std::endl;
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		GLchar* ProgramErrorMessage = new GLchar(InfoLogLength + 1);
		glGetProgramInfoLog(programID, InfoLogLength, NULL, ProgramErrorMessage);
		std::cout << ProgramErrorMessage << std::endl;
		return -1;
	}

	std::cout << "Succeed to link program." << std::endl;

	std::vector<GLuint>::iterator it;
	for	(it = shaderIds.begin(); it != shaderIds.end(); ++it)
	{
		glDetachShader(programID, *it);  
		glDeleteShader(*it);
	}

	return programID;
}
 


