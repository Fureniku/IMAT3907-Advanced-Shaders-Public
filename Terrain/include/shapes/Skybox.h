#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shape.h"

#include "texture/SkyboxTexture.h"
#include "texture/Texture.h"

class Skybox : public Shape {
private:
    SkyboxTexture skyboxTexture = SkyboxTexture(15);
    
public:
    Skybox();
    void draw(Shader& shader) override;
    std::vector<float> getRawVertices() override;
    std::vector<unsigned int> getRawIndices() override;
};
