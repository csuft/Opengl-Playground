#version 430 core

in vec4 vs_color;
out vec4 color;

void main()
{
	// Generate fragment color based on fragment coordinates.
	//color = vec4(sin(gl_FragCoord.x*0.25)*0.5 + 0.5, cos(gl_FragCoord.y*0.25)*0.5+0.5, sin(gl_FragCoord.x*0.15)*cos(gl_FragCoord.y*0.15), 1.0);

	// use color passed from vertex shader
	// OpenGL interpolates color values across the primitive.
	color = vs_color;
}