#version 450 core

// reading in a triangle, split tesselated triangels evenly in a counter-clockwise direction (ccw)
layout(triangles, equal_spacing, ccw) in;

// forward declare functions to perfrom interpolation with the barycentric coordinates from the Primitive Generator
vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2) ;
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2) ;

// unifrom matrices to perform transformations
// previously this would have been done in vertex shader
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int scale;

uniform sampler2D heightMap;

// read in vector arrays from previous shader
in vec3 posTC[];
in vec2 texCoordsTC[];

// pass along the interpolated values
out vec3 posES;
out vec2 texCoordsES;

out float heightCol;

void main() {
    // interpolate the normal and xyz position using the linear interpolation function
    // use 3D because they are in three dimensions; 2D also included for uv texture coordinates

    posES = interpolate3D(posTC[0], posTC[1], posTC[2]) ;
    texCoordsES = interpolate2D(texCoordsTC[0], texCoordsTC[1], texCoordsTC[2]) ;
    
    float height = texture(heightMap, texCoordsES).x;
    posES.y = height*scale;
    heightCol = height * 64 - 16;

    // transform vertex to clip space  - NOTE: WE NEED TO DO IT HERE NOW and not in vertex shader
    gl_Position = projection * view * vec4(posES, 1.0);
}


//basic linear interpolation
vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2) {
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2) {
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

