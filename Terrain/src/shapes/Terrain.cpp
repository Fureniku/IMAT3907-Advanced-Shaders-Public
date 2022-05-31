#include "shapes/Terrain.h"

#include <../../Dependancies/include/glad/glad.h>
#include <../../Dependancies/include/glm/gtc/matrix_transform.hpp>

#include "PerlinNoise.h"
#include "Properties.h"

//Terrain constructors
Terrain::Terrain(int widthIn, int stepSizeIn, glm::vec2 pos) {
	m_pos = pos;
	width = widthIn;
	stepSize = stepSizeIn;
	makeVertices(&vertices);
}

Terrain::Terrain() {
	width = 50;
	stepSize = 10;
	makeVertices(&vertices);
	setVAO();
}

unsigned int Terrain::setVAO() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	//xyz
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}

std::vector<float> Terrain::getVertices() {
	return vertices;
}

void Terrain::makeVertices(std::vector<float> *vertices) {
	for (int y = 0; y < width - 1; y++) {
		float  offSetY = y * stepSize;
		for (int x = 0; x < width - 1; x++) {
			float offSetX = x * stepSize;
			makeVertex(m_pos.x + offSetX,                m_pos.y + offSetY,            vertices);  // a
			makeVertex(m_pos.x + offSetX,              m_pos.y + offSetY + stepSize, vertices);  // b
			makeVertex(m_pos.x + offSetX + stepSize,   m_pos.y + offSetY,            vertices);   // c
			makeVertex(m_pos.x + offSetX + stepSize,   m_pos.y + offSetY,            vertices);  //d
			makeVertex(m_pos.x + offSetX,              m_pos.y + offSetY + stepSize, vertices);  //e
			makeVertex(m_pos.x + offSetX + stepSize, m_pos.y + offSetY + stepSize, vertices);  //f
		}
	}
}

void Terrain::makeVertex(int x, int y, std::vector<float> *vertices) {
	//x y z position
	vertices->push_back((float)x); //xPos
	vertices->push_back(0); //yPos - always 0 for now. Going to calculate this on GPU - can change to calclaute it here.
	vertices->push_back((float)y); //zPos

   // add texture coords
	vertices->push_back((float)x / (width*stepSize));
	vertices->push_back((float)y / (width*stepSize));
}

void Terrain::draw(Shader& shader) {
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_pos.x, 1, m_pos.y));
	shader.setMat4("model", model);
	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, Properties::wireframe ? GL_LINE : GL_FILL);
	glDrawArrays(GL_PATCHES, 0, getSize());
	model = glm::mat4(1.0f);
}

void Terrain::updatePos(glm::vec2 pos) {
	m_pos = pos;
	setVAO();
}