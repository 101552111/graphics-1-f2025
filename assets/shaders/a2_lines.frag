#version 430
in vec3 color;
out vec4 fragColor;

uniform vec3 u-color;
in vec3 world_color;

void main()
{
    fragColor = vec4(world_color, 1.0);
}
