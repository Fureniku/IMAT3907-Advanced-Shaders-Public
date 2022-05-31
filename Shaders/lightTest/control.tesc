// first step of tesselation shader
// tesselation control shader add/deletes control points and determines the tesselatation level
// patch has three control points here (three vertices for each triangle)


#version 450 core
layout (vertices =3) out;

float GetTessLevel(float a, float b);

uniform vec3 viewPos;

in vec3 posVS[] ;
in vec2 texCoordsVS[];

out vec3 posTC[] ;
out vec2 texCoordsTC[];

void main() {
    int tessLevel = 10;

    if (gl_InvocationID==0) {
        float eyeToVertexDist0 = distance(viewPos, posVS[0]);
        float eyeToVertexDist1 = distance(viewPos, posVS[1]);
        float eyeToVertexDist2 = distance(viewPos, posVS[2]);
        
        gl_TessLevelOuter[0] = GetTessLevel(eyeToVertexDist1, eyeToVertexDist2);
        gl_TessLevelOuter[1] = GetTessLevel(eyeToVertexDist2, eyeToVertexDist0);
        gl_TessLevelOuter[2] = GetTessLevel(eyeToVertexDist0, eyeToVertexDist1);
        gl_TessLevelInner[0] = gl_TessLevelOuter[2];
    }

    posTC[gl_InvocationID]  = posVS[gl_InvocationID];
    texCoordsTC[gl_InvocationID]  = texCoordsVS[gl_InvocationID];
}

float GetTessLevel(float a, float b) {
    float avgDistance = (a + b) / 2.0f;

    float max = 500;
    float min = 15;

    if (avgDistance < 15) {
        return 1000;
    } else if (avgDistance < 150) {
        return 5;
    } else if (avgDistance < 500) {
        return 2;
    } else {
        return 1;
    }
}

/*float GetTessLevel(float a, float b) {
    float eulerNumber = 2.71828;
    float avgDist = (a + b) / 2.0;
    float initialTL = 10;
    float RateOfDecay = -0.0035;
    float outcome = initialTL + (eulerNumber * (avgDist * RateOfDecay)); 

    return max(outcome, 1.0) ;
}*/