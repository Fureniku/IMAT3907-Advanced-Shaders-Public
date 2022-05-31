#version 330 core
#define numPointLights 2

//vec3 getDirectionalLight(vec3 norm, vec3 viewDir, float shadow);
vec3 getPointLight(int i, vec3 norm, vec3 viewDir);
//vec3 getSpotLight(int i, vec3 norm, vec3 viewDir);
float blinn(vec3 norm, vec3 viewDir, vec3 lightDir, float shine);

struct PointLight {
    vec3 pos;
    vec3 aCol;
    vec3 dCol;
    vec3 sCol;
    float Kc;
    float Kl;
    float Ke;

    float shine;
    float specStr;
};


uniform PointLight pLight[numPointLights];

out vec4 FragColor ;

in vec3 viewPos;
in vec2 texCoordsES;
in vec3 posES;
in float heightCol;

float ambientFactor = 0.5;

void main() {
    vec3 result = getPointLight(0, normalize(vec3(0.0)), viewPos);

    FragColor = vec4(result, 1.0);
}

/*vec3 getDirectionalLight(vec3 norm, vec3 viewDir, float shadow) {
    //ambient
    vec3 ambientColour = dLightCol * diffMapCol * ambientFactor;

    //diffuse
    float diffuseFactor = max(dot(norm, -dLightDir), 0.0);
    vec3 diffuseColour = dLightCol * diffMapCol * diffuseFactor;

    //specular
    float specBlinn = blinn(norm, viewDir, dLightDir, dLightShine);
    vec3 specularColour = dLightCol * specBlinn * specMapCol;

    return ambientColour + (1.0 - shadow) * (diffuseColour + specularColour);
}*/

vec3 getPointLight(int i, vec3 norm, vec3 viewDir) {
    float dist = length(pLight[i].pos - posES);
    float attn = 1.0/(pLight[i].Kc + (pLight[i].Kl*dist) + (pLight[i].Ke * (dist*dist)));
    vec3 pLightDir = normalize(pLight[i].pos - posES);

    //ambient
    vec3 ambientColor = pLight[i].aCol * ambientFactor *attn;

    //diffuse
    float diffuseFactor = max(dot(norm, pLightDir), 0.0);
    vec3 diffuseColor = pLight[i].dCol * diffuseFactor * attn;

    //specular
    float phong = blinn(norm, viewDir, pLightDir, pLight[i].shine);
    vec3 specularColour = pLight[i].sCol * phong * attn;

    return ambientColor + diffuseColor + specularColour;
}

/*vec3 getSpotLight(int i, vec3 norm, vec3 viewDir) {
    float dist = length(sLight[i].pos - posWS);
    float attn = 1.0/(sLight[i].Kc + (sLight[i].Kl*dist) + (sLight[i].Ke * (dist*dist)));
    vec3 sLightDir = normalize(sLight[i].pos - posWS);

    //diffuse
    float diffuseFactor = max(dot(norm, sLightDir), 0.0);
    vec3 diffuseColour = sLight[i].col * diffMapCol * diffuseFactor * attn;

    //specular
    float specBlinn = blinn(norm, viewDir, sLightDir, sLight[i].shine);
    vec3 specularColour = sLight[i].col * specBlinn * specMapCol * attn;

    float theta = dot(-sLightDir, normalize(sLight[i].dir));
    float illumination = clamp((theta - sLight[i].outerRad) / (sLight[i].innerRad - sLight[i].outerRad), 0.0, 1.0);

    diffuseColour = diffuseColour * illumination;
    specularColour = specularColour * illumination;

    return diffuseColour + specularColour;
}*/

float blinn(vec3 norm, vec3 viewDir, vec3 lightDir, float shine) {
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specularFactor = dot(norm, halfwayDir);
    return pow(max(specularFactor, 0.0), shine);
}