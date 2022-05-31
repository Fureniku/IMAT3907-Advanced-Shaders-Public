#pragma once
#include <string>
#include <vector>

#include "Texture.h"

class TextureRegistry {
private:
	int textureId = 0;
	std::vector<Texture> textures;

public:
	TextureRegistry();
	void registerTexture(std::string name);
	void registerTexture(std::string name, std::string path);

	Texture getTexture(int id) { return textures.at(id); }
	void activateAllTextures(Shader& shader);
	Texture getTextureFromName(std::string name);
};
