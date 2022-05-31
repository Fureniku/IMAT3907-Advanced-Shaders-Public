#version 410 core

in vec2 uv;
in vec2 textureCoords;
in vec3 camVec;
in vec3 fromLightVec;
out vec4 FragColor;

uniform sampler2D reflection;
uniform sampler2D refraction;
uniform sampler2D dudv;
uniform sampler2D dudvNormal;
uniform sampler2D depthMap;

uniform float screenW;
uniform float screenH;

uniform float moveFactor;
uniform float refractivity;

uniform vec3 dLightCol;

const float waveStrength = 0.02;
const float shineDamp = 20.0;
const float lightReflect = 0.6;

void main() {
    vec2 ndcXY = gl_FragCoord.xy;
    vec2 reflectTexCoords = vec2((ndcXY.x)/screenW, (-ndcXY.y)/screenH);
    vec2 refractTexCoords = vec2((ndcXY.x)/screenW,  (ndcXY.y)/screenH);

    float nearPlane = 0.1f;
    float farPlane = 1200.0f;
    
    float depth = texture(depthMap, refractTexCoords).r;
    float floorDistance = 2.0 * nearPlane * farPlane / (farPlane + nearPlane - (2.0 * depth - 1.0) * (farPlane - nearPlane));
    
    depth = gl_FragCoord.z;
    float waterDistance = 2.0 * nearPlane * farPlane / (farPlane + nearPlane - (2.0 * depth - 1.0) * (farPlane - nearPlane));
    
    float waterDepth = floorDistance - waterDistance;
    
    vec2 distortedTexCoords = texture(dudv, vec2(textureCoords.x*0.01 + moveFactor, textureCoords.y*0.01)).rg*0.1;
    distortedTexCoords = textureCoords*0.01 + vec2(distortedTexCoords.x, distortedTexCoords.y + moveFactor);
    vec2 totalDistortion = (texture(dudv, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength;

    reflectTexCoords += totalDistortion;
    reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
    reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);
    
    refractTexCoords += totalDistortion;
    refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

    vec4 reflectTex = texture(reflection, reflectTexCoords);
    vec4 refractTex = texture(refraction, refractTexCoords);

    vec4 normalMapColour = texture(dudvNormal, distortedTexCoords);
    vec3 normal = vec3(normalMapColour.r * 2.0- 1.0, normalMapColour.b * 3.0, normalMapColour.g* 2.0- 1.0);
    normal = normalize(normal);
    
    vec3 viewVector = normalize(camVec);
    float refractiveFactor = dot(viewVector, vec3(0.0, 1.0, 0.0));
    refractiveFactor = pow(refractiveFactor, refractivity);
    
    
    vec3 reflectedLight = reflect(normalize(fromLightVec), normal);
    float specular = max(dot(reflectedLight, viewVector), 0.0);
    specular = pow(specular, shineDamp);
    vec3 specularHighlights = dLightCol * specular * lightReflect;
    
    
    vec4 water = mix(reflectTex, refractTex, refractiveFactor);
    vec4 finalCol = mix(water, vec4(0.0, 0.3, 0.5, 1.0), 0.25) + vec4(specularHighlights, 0.0);
    finalCol.a = clamp(waterDepth/5.0, 0.0, 1.0);
    FragColor = finalCol;
}