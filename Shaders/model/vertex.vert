#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorms;
layout (location = 2) in vec2 aUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normalVS;
out vec2 vUV;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    normalVS = normalize(vec3(vec4(normalMatrix * aNorms, 0.0)));

    vUV = aUV;
}