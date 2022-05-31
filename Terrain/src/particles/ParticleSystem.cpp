#include "particles/ParticleSystem.h"

ParticleSystem::ParticleSystem(int n, int width, int height, glm::vec3 emitter) {
	
}

void ParticleSystem::run(Camera& camera) {
	renderParticles(camera);
	updatePositions();
}

void ParticleSystem::renderParticles(Camera& camera) {
	
}

void ParticleSystem::updatePositions() {
	//computeParticles->use();
	glDispatchCompute(workGroupSize, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void ParticleSystem::init() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
		
	glGenBuffers(1, &posSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, SIZE * sizeof(particle), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, posSSBO);
}