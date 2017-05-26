#version 430 core

layout(location=1) in vec3 vert_coords; 

out vec4 vert_out_color;

void main()
{
	gl_Position = vec4(vert_coords, 1.0f);
	vert_out_color = vec3(1.0f, 0.0f, 0.0f);
}