#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "shader.h"
#include "texture/Texture.h"

class Water {
private:
	glm::vec4 m_clipPlane;
	float scale = 1000.0f;
	float x = 0.0f;
	float y = 0.0f;
	float height = 0.0f;
	glm::vec2 m_pos;

	std::vector<float> vertices;
	std::vector<int> indices;

	unsigned int VAO;
	unsigned int VBO;

	Texture dudvTexture;
	Texture dudvNormalTexture;

	float waveSpeed = 0.03f;
	float moveFactor = 0.0f;
	
	int getSize() {	return vertices.size(); }
public:
	Water() = default;
	Water(float h, glm::vec3 clipPlane) {
		height = h;
		m_clipPlane = glm::vec4(clipPlane, height);

		dudvTexture = Texture("dudv", "..\\Resources\\dudv.png", 8);
		dudvNormalTexture = Texture("dudvNormal", "..\\Resources\\dudv_normal.png", 9);

		setVAO();
	}

	glm::vec4 getClipPlane() { return m_clipPlane; }

	glm::vec4 getFlippedClipPlane() {
		return {m_clipPlane.x * -1, m_clipPlane.y * -1, m_clipPlane.z * -1, m_clipPlane.w * -1};
	}

	unsigned int setVAO();
	float getHeight() { return height; }
	void updatePos(glm::vec2 pos);
	void draw(Shader& shader);
	float getWaveSpeed() { return waveSpeed; }
	float getMoveFactor() { return moveFactor; }
	void incrementMoveFactor(float time);
	Texture getDuDvTexture() { return dudvTexture; }
	Texture getDuDvNormalTexture() { return dudvNormalTexture; }
};
