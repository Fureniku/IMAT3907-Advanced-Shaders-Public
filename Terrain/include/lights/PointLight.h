#pragma once
#include "Light.h"
#include "shapes/Cube.h"

class PointLight : public Light {
private:
	glm::vec3 m_col;
protected:
	Cube cube;
public:
	PointLight(glm::vec3 pos, glm::vec3 col) :
		Light("pLight", pos, 1.0f, 0.22f, 0.22f, 128, 0.4), m_col(col) {
		cube = Cube(col, 0.25f, pos, false);
	}

	PointLight(glm::vec3 pos, glm::vec3 col, float cubeSize, bool cubeSpin) :
		Light("pLight", pos, 1.0f, 0.22f, 0.22f, 128, 0.4), m_col(col) {
		cube = Cube(col, cubeSize, pos, cubeSpin);
	}

	void setUniforms(int id, Shader& shader) override;
	void draw(Shader& shader);
	Cube getCube() { return cube; }
};
