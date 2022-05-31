#version 410 core

in vec3 posVS;
out vec4 FragColor;

uniform sampler2D computeHeight;

void main() {
    vec4 computeTex = texture(computeHeight, posVS.xz*0.1 + vec2(0.5, 0.5));
    FragColor = computeTex;
}