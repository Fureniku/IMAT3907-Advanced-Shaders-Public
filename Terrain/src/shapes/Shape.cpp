#include "shapes/Shape.h"

void Shape::extractVerts(std::vector<float> vertexData) {
	Vertex V;
	for (int i = 0; i < vertexData.size(); i += 8) {
		V.Position = glm::vec3(vertexData[i], vertexData[i + 1], vertexData[i + 2]);
		V.Normal = glm::vec3(vertexData[i + 3], vertexData[i + 4], vertexData[i + 5]);
		V.TexCoords = glm::vec2(vertexData[i + 6], vertexData[i + 7]);

		V.Tangent = glm::vec3(0.0f);
		V.Bitangent = glm::vec3(0.0f);

		vertices.push_back(V);
	}
}

void Shape::computeTanAndBiTan(std::vector<unsigned int> indicesData) {
	for (int i = 0; i < indicesData.size(); i += 3) {
		Vertex& a = vertices.at(indicesData[i]);
		Vertex& b = vertices.at(indicesData[i + 1]);
		Vertex& c = vertices.at(indicesData[i + 2]);

		glm::vec3 delta1 = b.Position - a.Position;
		glm::vec3 delta2 = c.Position - a.Position;

		glm::vec2 dUV1 = b.TexCoords - a.TexCoords;
		glm::vec2 dUV2 = c.TexCoords - a.TexCoords;

		float r = 1.0f / (dUV1.x * dUV2.y - dUV1.y * dUV2.x);

		glm::vec3 tangent = (delta1 * dUV2.y - delta2 * dUV1.y) * r;
		glm::vec3 biTangent = (delta2 * dUV1.x - delta1 * dUV2.x) * r;

		a.Tangent = a.Tangent + tangent;
		a.Bitangent = a.Bitangent + biTangent;

		b.Tangent = b.Tangent + tangent;
		b.Bitangent = b.Bitangent + biTangent;

		c.Tangent = c.Tangent + tangent;
		c.Bitangent = c.Bitangent + biTangent;
	}

	for (Vertex& V : vertices) {
		V.Tangent = glm::normalize(V.Tangent);
		V.Bitangent = glm::normalize(V.Bitangent);
	}
}

void Shape::calculateTanAndBiTan(std::vector<float> vertexData, std::vector<unsigned int> indicesData) {
	extractVerts(vertexData);
	computeTanAndBiTan(indicesData);
	unpackVertices();
}

void Shape::unpackVertices() {
	for (Vertex V : vertices) {
		int stride = 14;

		float vertexData[] = {
			V.Position.x, V.Position.y, V.Position.z,
			V.Normal.x, V.Normal.y, V.Normal.z,
			V.TexCoords.x, V.TexCoords.y,
			V.Tangent.x, V.Tangent.y, V.Tangent.z,
			V.Bitangent.x, V.Bitangent.y, V.Bitangent.z
		};

		for (int i = 0; i < stride; i++) {
			updatedVertexData.push_back(vertexData[i]);
		}
	}
}

std::vector<float> Shape::getUpdatedVertexData() {
	return updatedVertexData;
}

void Shape::initGL(std::vector<float> updatedVerts, std::vector<unsigned int> indicesData) {
	int length = indicesData.size() * sizeof(GLint);
	unsigned int* indData = &indicesData[0];

	glEnable(GL_DEPTH_TEST);

	// Create VAO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// fill VBO with vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, updatedVerts.size() * sizeof(GLfloat), updatedVerts.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, length, indData, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// uv attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// tan attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	// bitan attribute
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);
}
