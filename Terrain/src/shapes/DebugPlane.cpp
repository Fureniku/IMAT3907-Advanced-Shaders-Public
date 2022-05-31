#include "shapes/DebugPlane.h"
#include <../../Dependancies/include/glad/glad.h>
#include <../../Dependancies/include/glm/gtc/matrix_transform.hpp>

unsigned int DebugPlane::setVAO() {
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

void DebugPlane::draw() {
	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
