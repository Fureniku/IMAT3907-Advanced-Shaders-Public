#include "ComputeShader.h"

ComputeShader::ComputeShader(std::string name, glm::ivec2 size) {
	shaderName = name;
	m_size = size;
	std::string computePath = "..\\shaders\\compute\\" + name;
	std::string computeCode;

	try	{
		computeCode = shaderCodeString(computePath.c_str());
	} catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	unsigned int compute = shaderCode(computeCode, "COMPUTE", GL_COMPUTE_SHADER);

	ID = glCreateProgram();
	glAttachShader(ID, compute);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	
	glDeleteShader(compute);

	std::cout << "Loading " << shaderName << " complete." << std::endl << std::endl;
}

void ComputeShader::use() {
	glUseProgram(ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, compute_texture);
}

void ComputeShader::dispatch() {
	glDispatchCompute(m_size.x, m_size.y, 1);
}

void ComputeShader::wait() {
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void ComputeShader::createTexture(TextureManager* texMan) {
	compute_texture = texMan->createTexture(2000, 2000);
	glBindImageTexture(0, compute_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

void ComputeShader::bindTexture(int textureId) {
	glBindImageTexture(textureId, compute_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}