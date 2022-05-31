#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "shader.h"
#include "texture/Texture.h"

class DebugPlane {
private:
	float scale = 50.0f;
	float x = 0.0f;
	float y = 0.0f;
	float height = 0.0f;
	glm::vec2 m_pos;

	std::vector<float> vertices;
	std::vector<int> indices;

	unsigned int VAO;
	unsigned int VBO;
	
	int getSize() {	return vertices.size(); }
	
public:
	DebugPlane() = default;
	DebugPlane(float h) {
		height = h;
		
		setVAO();
	}

	unsigned int setVAO();
	float getHeight() { return height; }
	void draw();
};
