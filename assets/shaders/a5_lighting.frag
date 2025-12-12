#version 430
out vec4 fragColor;

in vec3 frag_position; // Fragment position in world-space
in vec3 frag_normal;   // Fragment normal in world-space
in vec2 uv;

uniform vec3 u_light_position;
uniform vec3 u_light_color;
uniform sampler2D u_sampler0;

void main()
{
    vec3 tex_col = texture(u_sampler0, uv).xyz;

    // FROM fragment TO light (AB = B - A)
    vec3 L = normalize(u_light_position - frag_position);
    vec3 N = normalize(frag_normal);
    float dotNL = max(dot(N, L), 0.0);

    vec3 ambient = u_light_color * 0.0; // TODO -- control ambient strength with a uniform!
    vec3 diffuse = u_light_color * dotNL;
    vec3 specular = u_light_color * 0.0; // TODO -- add the formula (dotVR) for specular!
    vec3 lighting = ambient + diffuse + specular;

    fragColor = vec4(tex_col * lighting, 1.0);
}
