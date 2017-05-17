#version 330 core

layout(location=0) in vec3 vertexPos_modelSpace;

void main()
{	
	gl_Position.xyz = vertexPos_modelSpace;
	gl_Position.w = 1.0;
}