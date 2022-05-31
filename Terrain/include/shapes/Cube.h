#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shape.h"
#include "texture/Texture.h"

class Cube : public Shape {
private:
    float m_size = 0.5f;
    glm::vec3 m_cubePos;
    glm::vec3 m_spinAxis = glm::vec3(2.0, 2.0, 2.0);
    glm::vec3 m_col = glm::vec3(1.0f);
	bool spin = false;

public:
    Cube() = default;
    Cube(glm::vec3 col, float cubeSize, glm::vec3 pos, bool shouldSpin);
    void draw(Shader& shader) override;
    inline void setCol(glm::vec3 col) { m_col = col; }
    inline glm::vec3 getCol() { return m_col; }
    std::vector<float> getRawVertices() override;
    std::vector<unsigned int> getRawIndices() override;
};
