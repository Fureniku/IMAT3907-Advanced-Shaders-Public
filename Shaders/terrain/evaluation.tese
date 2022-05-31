#version 450 core

// reading in a triangle, split tesselated triangels evenly in a counter-clockwise direction (ccw)
layout(triangles, equal_spacing, ccw) in;

// forward declare functions to perfrom interpolation with the barycentric coordinates from the Primitive Generator
vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2) ;
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2) ;

float noise(vec3 pos, int numOctaves);
float hash(float n);
float snoise(vec3 x);

// unifrom matrices to perform transformations
// previously this would have been done in vertex shader
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int scale;
uniform vec3 viewPos;
uniform bool algorithm;

uniform int heightGenerationMethod;

uniform sampler2D computeHeight;
uniform sampler2D heightMap;

// read in vector arrays from previous shader
in vec2 texCoordsTC[];
in vec3 posTC[];

// pass along the interpolated values
out vec3 posES;
out vec2 texCoordsES;
out float visibility;
out vec3 normalsES;

int octaves = 10;

void main() {
    // interpolate the normal and xyz position using the linear interpolation function
    // use 3D because they are in three dimensions; 2D also included for uv texture coordinates

    posES = interpolate3D(posTC[0], posTC[1], posTC[2]) ;
    texCoordsES = interpolate2D(texCoordsTC[0], texCoordsTC[1], texCoordsTC[2]) ;
    int off = 1;
    vec3 right = vec3(posES.x+off, posES.y, posES.z);
    vec3 left  = vec3(posES.x-off, posES.y, posES.z);
    vec3 up    = vec3(posES.x, posES.y, posES.z+off);
    vec3 down  = vec3(posES.x, posES.y, posES.z-off);
    
    float r = noise(right, octaves);
    float l = noise(left, octaves);
    float u = noise(up, octaves);
    float d = noise(down, octaves);
    normalsES = normalize(vec3(-(l-r), -2, -(d-u)));
    
    float height = texture(computeHeight, texCoordsES).x*2 - 1;
    
    if (heightGenerationMethod == 0) { //compute
        if (algorithm) {
            height = texture(computeHeight, texCoordsES).x * 2 - 1;
            posES.y = height*scale - scale;
        } else {
            height = texture(computeHeight, texCoordsES).x * 2 - 1;
            posES.y = height*scale + (scale/2);
        }
    } else if (heightGenerationMethod == 1) { //evaluation
        height = noise(posES, octaves) * 2 - 1;;
        posES.y = height*scale - scale;
    } else if (heightGenerationMethod == 2) { //heightMap
        height = texture(heightMap, texCoordsES).x * 2 - 1;
        posES.y = height*scale;
    }

    // transform vertex to clip space  - NOTE: WE NEED TO DO IT HERE NOW and not in vertex shader
    gl_Position = projection * view * vec4(posES, 1.0);
    
    float gradiant = 3.5f;
    float density = 0.002f;
    
    float distFromCam = distance(viewPos, posES);
    visibility = exp(-pow((distFromCam*density), gradiant));
    visibility = clamp(visibility, 0.0, 1.0);
}


//basic linear interpolation
vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2) {
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2) {
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

float noise(vec3 pos, int numOctaves) {
    float total = 0.0f;
    float freq = 0.005f;
    
    float maxAmp = 0.0f;
    float amp = 10.0f;
    
    for (int i = 0; i < numOctaves; i++) {
        total += snoise(pos * freq) * amp;
        freq *= 2;
        amp /= 2;
        maxAmp += amp;
    }
    
    return total / maxAmp;
}

float hash(float n) {
    return fract(sin(n) * 753.5453123);
}

float snoise(vec3 x) {
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f * f * (3.0f - (2.0f * f));

    float n = p.x + p.y * 157.0f + 113.0f * p.z;
    return mix(mix(mix(hash(n + 0.0f), hash(n + 1.0f), f.x),
        mix(hash(n + 157.0f), hash(n + 158.0f), f.x), f.y),
        mix(mix(hash(n + 113.0f), hash(n + 114.0f), f.x),
            mix(hash(n + 270.0f), hash(n + 271.0f), f.x), f.y), f.z);
}
