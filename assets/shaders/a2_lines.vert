#version 430
layout (location = 0) in vec2 vPos;

uniform mat4 u_mvp;     // world * view * proj
out vec3 world_color;  // Colour based on World-space posision 

void main()
{
    vec4 pos = vec4(vPos, 0.0, 1.0);
    world_color = pos.xyz;

    gl_Position = u_mvp * pos;
}
