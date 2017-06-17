#version 430 core

layout(location=0) in vec3 vertexPos_modelSpace;

out vec4 vs_color;

void main()
{	
	gl_Position.xyz = vertexPos_modelSpace;
	gl_Position.w = 1.0;

	const vec4 colors[] = vec4[3](
	vec4(1.0, 0.0, 0.0, 1.0),
	vec4(0.0, 1.0, 0.0, 1.0),
	vec4(0.0, 0.0, 1.0, 1.0));

	vs_color = colors[gl_VertexID];
}