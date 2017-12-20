#version 330 core

in vec2 vert_out_tex; 
out vec4 frag_color;

uniform sampler2D texSamplerY;
uniform sampler2D texSamplerU;
uniform sampler2D texSamplerV;

void main(void)
{
    vec3 yuv, rgb;
	yuv.x = texture2D(texSamplerY, vert_out_tex).r;
	yuv.y = texture2D(texSamplerU, vert_out_tex).r - 0.5;
	yuv.z = texture2D(texSamplerV, vert_out_tex).r - 0.5;
	rgb = mat3(1, 1, 1,  0, -0.39465, 2.03211, 1.13983, -0.58060, 0) * yuv;
	
    frag_color = vec4(rgb, 1.0);
}