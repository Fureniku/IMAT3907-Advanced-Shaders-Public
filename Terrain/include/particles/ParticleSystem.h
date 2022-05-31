#pragma once

#include "Camera.h"
#include "glm/glm.hpp"

#define SIZE 1024*1000

struct particle {
	glm::vec4 position;
	glm::vec4 velocities;
	glm::vec4 col;
};

class ParticleSystem {
private:
	unsigned int VAO, posSSBO;
	int workGroupSize = SIZE / 1024;
public:
	ParticleSystem(int n, int width, int height, glm::vec3 emitter);
	
	void init();
	void run(Camera& camera);
	void renderParticles(Camera& camera);
	void updatePositions();
};