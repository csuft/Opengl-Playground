#version 330 core

in vec2 vert_out_tex; 

out vec3 frag_color;

uniform sampler2D texSampler;

void main(void)
{
    frag_color = texture(texSampler, vert_out_tex).rgb;
}