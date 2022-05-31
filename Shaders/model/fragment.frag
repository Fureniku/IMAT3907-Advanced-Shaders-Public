#version 410 core

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

in vec3 normalVS;
in vec2 vUV;

void main()
{
    FragColor = texture(texture_diffuse1, vUV);

}

