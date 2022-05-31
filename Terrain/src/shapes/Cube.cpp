#include "shapes/Cube.h"
#include <GLFW/glfw3.h>

Cube::Cube(glm::vec3 col, float cubeSize, glm::vec3 pos, bool shouldSpin) {
	m_col = col;
	m_size = cubeSize;
	m_cubePos = pos;
	spin = shouldSpin;
	
	calculateTanAndBiTan(getRawVertices(), getRawIndices());
	std::vector<float> updatedCubeVerts = getUpdatedVertexData();
	
	initGL(updatedCubeVerts, getRawIndices());
}

std::vector<float> Cube::getRawVertices() {
	std::vector<float> cubeVertices {
		//back
		 m_size, -m_size, -m_size,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, //0
		-m_size, -m_size, -m_size,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		-m_size,  m_size, -m_size,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		 m_size,  m_size, -m_size,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

		//front
		-m_size, -m_size,  m_size,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, //4
		 m_size, -m_size,  m_size,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		 m_size,  m_size,  m_size,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		-m_size,  m_size,  m_size,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,

		//left
		-m_size,  m_size,  m_size, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, //8
		-m_size,  m_size, -m_size, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-m_size, -m_size, -m_size, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-m_size, -m_size,  m_size, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

		//right
		m_size,  m_size, -m_size,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,  //12
		m_size,  m_size,  m_size,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		m_size, -m_size,  m_size,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		m_size, -m_size, -m_size,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

		//bottom
		 m_size, -m_size, -m_size,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, //16
		-m_size, -m_size, -m_size,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		-m_size, -m_size,  m_size,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		 m_size, -m_size,  m_size,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,

		//top	
		-m_size,  m_size, -m_size,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, //20
		 m_size,  m_size, -m_size,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		 m_size,  m_size,  m_size,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		-m_size,  m_size,  m_size,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
	};

	return cubeVertices;
}

std::vector<unsigned int> Cube::getRawIndices() {
	std::vector<unsigned int> cubeIndices {
		1,2,3,
		1,3,0,

		5,6,7,
		5,7,4,

		11,8,9,
		11,9,10,

		15,12,13,
		15,13,14,

		19,18,17,
		19,17,16,

		23,22,21,
		23,21,20
	};
	return cubeIndices;
}

void Cube::draw(Shader& shader) {
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, m_cubePos);
	if (spin) { model = glm::rotate(model, (float)(glfwGetTime()), m_spinAxis); }
	
	shader.use();
	shader.setMat4("model", model);
	shader.setVec3("lightColour", m_col);
	
	glBindVertexArray(VAO);  // bind and draw cube
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	model = glm::mat4(1.0f);
}