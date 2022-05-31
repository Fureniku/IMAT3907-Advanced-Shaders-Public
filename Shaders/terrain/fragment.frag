#version 330 core
#define MAX_LIGHTS 32
vec3 getCol();
vec4 triPlaneTexture(sampler2D textureIn, float textureSize);
vec3 getTexture();

vec3 getDirectionalLight(vec3 norm, vec3 viewDir, float shadow, vec3 diffuseCol);
vec3 getPointLight(int i, vec3 norm, vec3 viewDir, vec3 diffuseCol);
vec3 getSpotLight(int i, vec3 norm, vec3 viewDir, vec3 diffuseCol);
vec3 getNormalTexture();
//float getSpecularCol();
float blinn(vec3 norm, vec3 viewDir, vec3 lightDir);

in vec2 texCoordsES;
in vec3 posGS;
in vec3 gNormals;
in float visibilityG;

out vec4 FragColor ;

struct PointLight { //5
    vec3 pos;
    vec3 col;
    float Kc;
    float Kl;
    float Ke;
};

struct SpotLight { //8
    vec3 pos;
    vec3 dir;
    vec3 col;
    float Kc;
    float Kl;
    float Ke;

    float innerRad;
    float outerRad;
};

//light uniforms
uniform int numPointLights;
uniform int numSpotLights;
uniform PointLight pLight[MAX_LIGHTS];
uniform SpotLight sLight[MAX_LIGHTS];

uniform vec3 dLightCol;
uniform vec3 dLightDir;
uniform float shine;
uniform float specStr;

//world setting uniforms
uniform int scale;
uniform vec3 viewPos;
uniform vec3 sky;
uniform bool useHeightTexture;
uniform bool fog;
uniform bool camUnderWater;

//texture uniforms
uniform sampler2D grassTextureDiffuse;
uniform sampler2D grassTextureNormal;
//uniform sampler2D grassTextureHeight;
//uniform sampler2D grassTextureSpecular;

uniform sampler2D sandTextureDiffuse;
uniform sampler2D sandTextureNormal;
//uniform sampler2D sandTextureHeight;
//uniform sampler2D sandTextureSpecular;

uniform sampler2D beachTextureDiffuse;
uniform sampler2D beachTextureNormal;
//uniform sampler2D beachTextureHeight;
//uniform sampler2D beachTextureSpecular;

uniform sampler2D grassRockTextureDiffuse;
uniform sampler2D grassRockTextureNormal;
//uniform sampler2D grassRockTextureHeight;
//uniform sampler2D grassRockTextureSpecular;

uniform sampler2D rockTextureDiffuse;
uniform sampler2D rockTextureNormal;
//uniform sampler2D rockTextureHeight;
//uniform sampler2D rockTextureSpecular;

uniform sampler2D snowTextureDiffuse;
uniform sampler2D snowTextureNormal;
//uniform sampler2D snowTextureHeight;
//uniform sampler2D snowTextureSpecular;

float ambientFactor = 1.0;
//float specularCol = 1.0f;

void main() {
    float h = posGS.y;
    vec3 diffuseCol = getCol();
    vec3 normalCol = gNormals; //getNormalCol();

    if (useHeightTexture) {
        diffuseCol = getTexture();
        normalCol = getNormalTexture();
        //specularCol = getSpecularCol();
    } else {
        diffuseCol = getCol();
    }
    
    vec3 result = getDirectionalLight(normalCol, viewPos, 1, diffuseCol);
    
    for (int i = 0; i < numPointLights; i++) {
        result = result + getPointLight(i, normalCol, viewPos, diffuseCol);
    }

    for (int i = 0; i < numSpotLights; i++) {
        result = result + getSpotLight(i, normalCol, viewPos, diffuseCol);
    }
    
    if (camUnderWater) {
        result = result + vec3(0.0, 0.0, 0.25);
    }
    
    FragColor = vec4(result,1.0) ;
    if (fog) {
        FragColor = mix(vec4(sky, 1.0), FragColor, visibilityG);
    }
}

vec4 triPlaneTexture(sampler2D textureIn, float textureSize) {
    vec3 blendPercent = normalize(abs(gNormals));
    float b = (blendPercent.x + blendPercent.y + blendPercent.z);
    blendPercent = blendPercent/vec3(b);
    
    vec4 xAxis = texture(textureIn, posGS.yz*textureSize);
    vec4 yAxis = texture(textureIn, posGS.xz*textureSize);
    vec4 zAxis = texture(textureIn, posGS.xy*textureSize);
    
    return xAxis*blendPercent.x + yAxis*blendPercent.y + zAxis*blendPercent.z;
}

vec3 getTexture() {
    float height = posGS.y;
    
    vec3 grass = triPlaneTexture(grassTextureDiffuse, 0.1f).xyz;
    vec3 sand = triPlaneTexture(sandTextureDiffuse, 0.1f).xyz;
    vec3 beach = triPlaneTexture(beachTextureDiffuse, 0.1f).xyz;
    vec3 grassRock = triPlaneTexture(grassRockTextureDiffuse, 0.1f).xyz;
    vec3 rock = triPlaneTexture(rockTextureDiffuse, 0.001f).xyz;
    vec3 snow = triPlaneTexture(snowTextureDiffuse, 0.1f).xyz;
    
    vec3 colour = grass;
    
    if (height < 0.0) {
        colour = vec3(mix(sand, beach, smoothstep(-3.0, 0.0, height)).rgb);
    } else if (height < 5.0) {
        colour = vec3(mix(beach, grass, smoothstep(2.0, 5.0, height)).rgb);
    } else if (height < 30.0) {
        colour = vec3(mix(grass, grassRock, smoothstep(20.0, 30.0, height)).rgb);
    } else if (height < 75.0) {
        colour = vec3(mix(grassRock, rock, smoothstep(30.0, 40.0, height)).rgb);
    } else if (height < 100.0) {
        colour = vec3(mix(rock, snow, smoothstep(80.0, 95.0, height)).rgb);
    } else {
        colour = snow;
    }
    
    return colour;
}

vec3 getCol() {
    float height = posGS.y;

    vec4 grass = vec4(0.357, 0.411, 0.173, 0.0);
    vec4 sand = vec4(0.867, 0.741, 0.576, 0.0);
    vec4 beach = vec4(0.890, 0.847, 0.792, 0.0);
    vec4 grassRock = vec4(0.576, 0.521, 0.407, 0.0);
    vec4 rock = vec4(0.498, 0.450, 0.431, 0.0);
    vec4 snow = vec4(0.9, 0.9, 0.9, 0.0);
    
    vec3 colour = grass.rgb;

    if (height < 0.0) {
        colour = vec3(mix(sand, beach, smoothstep(-3.0, 0.0, height)).rgb);
    } else if (height < 5.0) {
        colour = vec3(mix(beach, grass, smoothstep(2.0, 5.0, height)).rgb);
    } else if (height < 30.0) {
        colour = vec3(mix(grass, grassRock, smoothstep(20.0, 30.0, height)).rgb);
    } else if (height < 75.0) {
        colour = vec3(mix(grassRock, rock, smoothstep(30.0, 40.0, height)).rgb);
    } else if (height < 100.0) {
        colour = vec3(mix(rock, snow, smoothstep(80.0, 95.0, height)).rgb);
    } else {
        colour = snow.rgb;
    }
    
    return colour;
}

vec3 getNormalTexture() {
    float height = posGS.y;

    vec3 grass = triPlaneTexture(grassTextureNormal, 0.1f).xyz;
    vec3 sand = triPlaneTexture(sandTextureNormal, 0.1f).xyz;
    vec3 beach = triPlaneTexture(beachTextureNormal, 0.1f).xyz;
    vec3 grassRock = triPlaneTexture(grassRockTextureNormal, 0.1f).xyz;
    vec3 rock = triPlaneTexture(rockTextureNormal, 0.001f).xyz;
    vec3 snow = triPlaneTexture(snowTextureNormal, 0.1f).xyz;

    vec3 colour = grass;

    if (height < 0.0) {
        colour = vec3(mix(sand, beach, smoothstep(-3.0, 0.0, height)).rgb);
    } else if (height < 5.0) {
        colour = vec3(mix(beach, grass, smoothstep(2.0, 5.0, height)).rgb);
    } else if (height < 30.0) {
        colour = vec3(mix(grass, grassRock, smoothstep(20.0, 30.0, height)).rgb);
    } else if (height < 75.0) {
        colour = vec3(mix(grassRock, rock, smoothstep(30.0, 40.0, height)).rgb);
    } else if (height < 100.0) {
        colour = vec3(mix(rock, snow, smoothstep(80.0, 95.0, height)).rgb);
    } else {
        colour = snow;
    }

    colour = colour * 2 - 1;
    colour = normalize(colour);
    return colour;
}

/*float getSpecularCol() {
    float height = posGS.y;

    vec3 grass = triPlaneTexture(grassTextureSpecular, 0.1f).xyz;
    vec3 sand = triPlaneTexture(sandTextureSpecular, 0.1f).xyz;
    vec3 beach = triPlaneTexture(beachTextureSpecular, 0.1f).xyz;
    vec3 grassRock = triPlaneTexture(grassRockTextureSpecular, 0.1f).xyz;
    vec3 rock = triPlaneTexture(rockTextureSpecular, 0.001f).xyz;
    vec3 snow = triPlaneTexture(snowTextureSpecular, 0.1f).xyz;

    vec3 colour = grass;

    if (height < 0.0) {
        colour = vec3(mix(sand, beach, smoothstep(-3.0, 0.0, height)).rgb);
    } else if (height < 5.0) {
        colour = vec3(mix(beach, grass, smoothstep(2.0, 5.0, height)).rgb);
    } else if (height < 30.0) {
        colour = vec3(mix(grass, grassRock, smoothstep(20.0, 30.0, height)).rgb);
    } else if (height < 75.0) {
        colour = vec3(mix(grassRock, rock, smoothstep(30.0, 40.0, height)).rgb);
    } else if (height < 100.0) {
        colour = vec3(mix(rock, snow, smoothstep(80.0, 95.0, height)).rgb);
    } else {
        colour = snow;
    }
    
    return colour.x;
}*/

//everything below here is taken from my IMAT3906 submission
vec3 getDirectionalLight(vec3 norm, vec3 viewDir, float shadow, vec3 diffuseCol) {
    //ambient
    vec3 ambientColour = dLightCol * diffuseCol * ambientFactor;

    //diffuse
    float diffuseFactor = max(dot(norm, -dLightDir), 0.0);
    vec3 diffuseColour = dLightCol * diffuseCol * diffuseFactor;

    //specular
    float specBlinn = blinn(norm, viewDir, dLightDir);
    vec3 specularColour = dLightCol * specBlinn;

    return (ambientColour + (1.0 - shadow) * (diffuseColour + specularColour)) * 0.5;
}

vec3 getPointLight(int i, vec3 norm, vec3 viewDir, vec3 diffuseCol) {
    float dist = length(pLight[i].pos - posGS);
    float attn = 1.0/(pLight[i].Kc + (pLight[i].Kl*dist) + (pLight[i].Ke * (dist*dist)));
    vec3 pLightDir = normalize(pLight[i].pos - posGS);

    //ambient
    vec3 ambientColor = pLight[i].col * diffuseCol * ambientFactor *attn;

    //diffuse
    float diffuseFactor = max(dot(norm, pLightDir), 0.0);
    vec3 diffuseColor = pLight[i].col * diffuseCol * diffuseFactor * attn;

    //specular
    float phong = blinn(norm, viewDir, pLightDir);
    vec3 specularColour = pLight[i].col * phong * attn;

    return ambientColor + diffuseColor + specularColour;
}

vec3 getSpotLight(int i, vec3 norm, vec3 viewDir, vec3 diffuseCol) {
    float dist = length(sLight[i].pos - posGS);
    float attn = 1.0/(sLight[i].Kc + (sLight[i].Kl*dist) + (sLight[i].Ke * (dist*dist)));
    vec3 sLightDir = normalize(sLight[i].pos - posGS);

    //diffuse
    float diffuseFactor = max(dot(norm, sLightDir), 0.0);
    vec3 diffuseColour = sLight[i].col * diffuseFactor * attn;

    //specular
    float specBlinn = blinn(norm, viewDir, sLightDir);
    vec3 specularColour = sLight[i].col * specBlinn * attn;

    float theta = dot(-sLightDir, normalize(sLight[i].dir));
    float illumination = clamp((theta - sLight[i].outerRad) / (sLight[i].innerRad - sLight[i].outerRad), 0.0, 1.0);

    diffuseColour = diffuseColour * illumination;
    specularColour = specularColour * illumination;

    return diffuseColour + specularColour;
}

float blinn(vec3 norm, vec3 viewDir, vec3 lightDir) {
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specularFactor = dot(norm, halfwayDir);
    return pow(max(specularFactor, 0.0), (shine*2)/1);
}