#version 120
// Input vertex data, different for all executions of this shader.
attribute vec3 vertexPosition_modelspace;

// Output data ; will be interpolated for each fragment.
varying vec2 UV;

uniform mat4 projMatrix;
uniform mat4 mvMatrix;

void main()
{
	gl_Position = projMatrix * mvMatrix * vec4(vertexPosition_modelspace, 1);
}