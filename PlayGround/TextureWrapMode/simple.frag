#version 430 core

in vec3 vert_out_color;
out vec4 frag_color;

void main(void)
{
    frag_color = vec4(vert_out_color, 1.0f);
}