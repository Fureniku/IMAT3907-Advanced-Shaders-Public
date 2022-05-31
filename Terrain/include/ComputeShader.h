#pragma once
#include "Shader.h"
#include "texture/TextureManager.h"

class ComputeShader : public Shader {
private:
	glm::ivec2 m_size;
	unsigned int compute_texture;
	
public:
	ComputeShader() = default;
	ComputeShader(std::string name, glm::ivec2 size);

	unsigned int getTexture() { return compute_texture; }
	void use() override;
	void createTexture(TextureManager* texMan);
	void bindTexture(int textureId);
	void dispatch();
	void wait();
};
