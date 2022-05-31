#include "texture/TextureRegistry.h"

TextureRegistry::TextureRegistry() {
	std::cout << "Initializiing texture registry" << std::endl;
	registerTexture("grassTexture");
	registerTexture("sandTexture");
	registerTexture("beachTexture");
	registerTexture("grassRockTexture");
	registerTexture("rockTexture");
	registerTexture("snowTexture");

}

void TextureRegistry::registerTexture(std::string name) {
	registerTexture(name+"Diffuse", "..\\Resources\\" + name + "\\diffuse.jpg");
	registerTexture(name+"Normal", "..\\Resources\\" + name + "\\normal.jpg");
	//registerTexture(name+"Height", "..\\Resources\\" + name + "\\height.jpg");
	//registerTexture(name+"Specular", "..\\Resources\\" + name + "\\specular.jpg");
}

void TextureRegistry::registerTexture(std::string name, std::string path) {
	textures.emplace_back(name, path, textureId);
	textureId++;
}

void TextureRegistry::activateAllTextures(Shader& shader) {
	for (int i = 0; i < textures.size(); i++) {
		textures.at(i).activateTexture(shader);
	}
}

Texture TextureRegistry::getTextureFromName(std::string name) {
	for (int i = 0; i < textures.size(); i++) {
		if (textures.at(i).getName() == name) {
			return textures.at(i);
		}
	}
	std::cout << "No texture with name " << name << " found; returning first value." << std::endl;
	return textures.at(0);
}
