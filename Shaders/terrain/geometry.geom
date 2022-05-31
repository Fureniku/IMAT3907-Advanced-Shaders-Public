#version 330 core

layout(triangles) in ;
layout(triangle_strip, max_vertices = 3) out ;
vec3 getNormal() ;

in vec3 posES[] ;
in float visibility[];
in vec3 normalsES[];

out vec3 posGS;
out vec3 gNormals;
out float visibilityG;

uniform bool flatShading;
uniform vec4 plane;
uniform mat4 model;

void main() {
    for(int i = 0 ; i < 3; i++) {
        gl_Position = gl_in[i].gl_Position;
        gl_ClipDistance[0] = dot(vec4(posES[i], 1.0), plane);
        posGS = posES[i] ;
        if (flatShading) {
            gNormals = getNormal();
        } else {
            gNormals = normalsES[i];
        }
        
        visibilityG = visibility[i];
        EmitVertex() ;
    }
    EndPrimitive() ;
}


vec3 getNormal() {
    vec3 a = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
    return normalize(cross(a, b));
}