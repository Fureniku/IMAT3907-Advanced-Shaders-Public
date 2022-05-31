#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 clipSpace;
out vec2 textureCoords;
out vec3 camVec;
out vec3 fromLightVec;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

uniform vec3 dLightPos;

const float tiling = 5.0;

void main() {
    vec4 worldPos = model * vec4(aPos, 1.0);
    camVec = viewPos - worldPos.xyz;
    gl_Position = projection * view * worldPos;
    textureCoords = vec2(aPos.x/2.0 + 0.5, aPos.z/2.0 + 0.5) * tiling;
    fromLightVec = worldPos.xyz - dLightPos;
}