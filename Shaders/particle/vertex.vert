#version 430 core

struct pos { 
    vec4 position;
    vec4 velocities;
    vec4 col;
};

layout (std140, binding = 1) buffer posSSBO {
    pos aPos[];
};

uniform mat4 model;

out float age;
out vec4 col;

void main() {
    age = aPos[gl_VertexID].position.z;
    vec3 posXYZ = aPos[gl_VertexID].position.xyz;
    col = aPos[gl_VertexID].cols;
    gl_Position = model*vec4(posXYZ, 1.0);
}