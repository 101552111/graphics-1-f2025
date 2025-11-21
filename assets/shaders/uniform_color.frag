#version 460 core

uniform vec3 u_color;

//step2
//uniform float u_intensity;

out vec4 FragColor;

void main()
{
//step3
    FragColor = vec4(u_color, 1.0);
}
