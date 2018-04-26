#version 330 core

uniform mat4 MVP;

layout(location=1) in vec3 vert_coords;
layout(location=2) in vec2 tex_coords;
 
out vec2 vert_out_tex;

void main()
{
	gl_Position = vec4(vert_coords, 1.0f); 
	vert_out_tex = tex_coords;
}