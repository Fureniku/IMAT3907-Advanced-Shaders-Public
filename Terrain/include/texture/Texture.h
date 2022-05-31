#pragma once

#include <../../Dependancies/include/glad/glad.h>
#include <../../Dependancies/include/glm/glm.hpp>
#include "stb_image.h"
#include "shader.h"
#include <iostream>
#include <string>

class Texture {
protected:
	std::string m_name;
	std::string m_texturePath;
	unsigned int m_texture;
	int m_id;

public:
	Texture() = default;
	Texture(std::string name, std::string path, int id) {
		m_name = name;
		m_texturePath = path;
		m_id = id;

		m_texture = loadTexture(m_texturePath.c_str());
	}
	
	virtual void activateTexture(Shader& shader);
	unsigned int loadTexture(char const* path);
	std::string getName() { return m_name; }
};
