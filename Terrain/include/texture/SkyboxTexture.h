#pragma once

#include "Texture.h"
#include <vector>

class SkyboxTexture : Texture {

public:
    SkyboxTexture() = default;
    SkyboxTexture(int id) {
        m_name = "skybox";
        m_texturePath = "..\\Resources\\skybox\\";
        m_id = id;

        std::vector<std::string> faces;
        faces.push_back(m_texturePath + "right.jpg");
        faces.push_back(m_texturePath + "left.jpg");
        faces.push_back(m_texturePath + "top.jpg");
        faces.push_back(m_texturePath + "bottom.jpg");
        faces.push_back(m_texturePath + "front.jpg");
        faces.push_back(m_texturePath + "back.jpg");

        m_texture = loadTexture(faces);
    }
	
    unsigned int loadTexture(std::vector<std::string> faces);

    unsigned int getTexture() { return m_texture; }
};