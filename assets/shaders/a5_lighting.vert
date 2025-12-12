#version 430
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTcoord;
layout (location = 2) in vec3 vNorm;

uniform mat4 u_world;
uniform mat4 u_mvp;

out vec3 frag_position; // Fragment position in world-space
out vec3 frag_normal;   // Fragment normal in world-space
out vec2 uv;

void main()
{
    frag_position = (u_world * vec4(vPos, 1.0)).xyz;
    frag_normal = mat3(transpose(inverse(u_world))) * vNorm;
    uv = vTcoord;
    gl_Position = u_mvp * vec4(vPos, 1.0);
}
