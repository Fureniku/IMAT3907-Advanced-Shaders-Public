#pragma once
#include <vector>

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

class Shape {
protected:
    std::vector<Vertex> vertices;
    std::vector<float> updatedVertexData;;
    unsigned int VBO, EBO, VAO;
    int stride = 14;

    void extractVerts(std::vector<float> vertexData);
    void computeTanAndBiTan(std::vector<unsigned int> indicesData);
public:
    virtual void draw(Shader& shader) = 0;
    virtual std::vector<float> getRawVertices() = 0;
    virtual std::vector<unsigned int> getRawIndices() = 0;

    void calculateTanAndBiTan(std::vector<float> vertexData, std::vector<unsigned int> indicesData);
    void unpackVertices();
    std::vector<float> getUpdatedVertexData();
    void initGL(std::vector<float> updatedVerts, std::vector<unsigned int> indicesData);
};