// first step of tesselation shader
// tesselation control shader add/deletes control points and determines the tesselatation level
// patch has three control points here (three vertices for each triangle)


#version 450 core
layout (vertices =3) out;

float GetTessLevel(float a, float b);

uniform vec3 viewPos;

in vec2 texCoordsVS[];
in vec3 posVS[] ;

out vec2 texCoordsTC[];
out vec3 posTC[] ;

void main() {
    float eyeToVertexDist0 = distance(viewPos, posVS[0]);
    float eyeToVertexDist1 = distance(viewPos, posVS[1]);
    float eyeToVertexDist2 = distance(viewPos, posVS[2]);

    if (gl_InvocationID==0) {
        gl_TessLevelOuter[0] = GetTessLevel(eyeToVertexDist1, eyeToVertexDist2);
        gl_TessLevelOuter[1] = GetTessLevel(eyeToVertexDist2, eyeToVertexDist0);
        gl_TessLevelOuter[2] = GetTessLevel(eyeToVertexDist0, eyeToVertexDist1);
        gl_TessLevelInner[0] = gl_TessLevelOuter[2];
    }

    posTC[gl_InvocationID]  = posVS[gl_InvocationID];
    texCoordsTC[gl_InvocationID]  = texCoordsVS[gl_InvocationID];
}

float GetTessLevel(float a, float b) {
    float averageDistance = (a + b) / 2.0f;
    float tessellationLevel = (exp(-0.01f * averageDistance)) * 100.0f;
    if (tessellationLevel > 1.0f) {
        return tessellationLevel;
    } else {
        return 1.0f;
    }
}