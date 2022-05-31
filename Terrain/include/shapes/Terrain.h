#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <../../Dependancies/include/glm/glm.hpp>

#include "PerlinNoise.h"
#include "Shader.h"

class Terrain {
public:
	Terrain(int widthIn, int stepSizeIn, glm::vec2 pos);
	Terrain();
	unsigned int setVAO();
	int getWorldSize() { return 200 * stepSize - stepSize; }
	void draw(Shader& shader);
	glm::vec2 getPos() { return m_pos; }

	int getSize() {	return vertices.size();	}
	void updatePos(glm::vec2 pos);
	
private:
	glm::vec2 m_pos;
	std::vector<float> vertices;
	unsigned int VAO, VBO;
	int width;
	int stepSize;
	void makeVertices(std::vector<float> *vertices);
	void makeVertex(int x, int y, std::vector<float> *vertices);
	std::vector<float> getVertices();
	PerlinNoise perlin;
};
#endif












