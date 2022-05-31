#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 2) out;

vec3 getSurfaceNormal();

in vec3 normalVS[];
uniform mat4 projection;
float normLength = 0.5f;

void main() {
    for (int i = 0; i < gl_in.length(); i++) {
        gl_Position = projection * gl_in[i].gl_Position;
        EmitVertex();
        vec3 normal = getSurfaceNormal();
        gl_Position = projection * (gl_in[i].gl_Position + vec4(normalVS[i], 0.0) * normLength);
        EmitVertex();
        EndPrimitive();
    }
}

vec3 getSurfaceNormal() {
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}