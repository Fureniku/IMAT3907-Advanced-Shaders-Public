#include "shapes/Water.h"
#include <../../Dependancies/include/glad/glad.h>
#include <../../Dependancies/include/glm/gtc/matrix_transform.hpp>

unsigned int Water::setVAO() {
	vertices = {
		x - scale, height, y + scale, 0.0f, 1.0f,
		x + scale, height, y + scale, 1.0f, 1.0f,
		x - scale, height, y - scale, 0.0f, 0.0f,
		x + scale, height, y - scale, 1.0f, 0.0f,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (vertices.size() * sizeof(float)), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	return VAO;
}

void Water::draw(Shader& shader) {
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_pos.x, 1, m_pos.y));
	shader.setMat4("model", model);
	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	model = glm::mat4(1.0f);
}

void Water::incrementMoveFactor(float time) {
	moveFactor += waveSpeed * time;
	if (moveFactor > 1) moveFactor = 0;
}

void Water::updatePos(glm::vec2 pos) {
	m_pos = pos;
	setVAO();
}