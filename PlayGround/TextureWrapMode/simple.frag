#version 330 core
 
in vec3 vert_out_color;

out vec3 frag_color;

void main(void)
{
    frag_color = vert_out_color;
}