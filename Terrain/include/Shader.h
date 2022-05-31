#ifndef SHADER_H
#define SHADER_H

#include <../../Dependancies/include/glad/glad.h>
#include <../../Dependancies/include/glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
enum EnumShaderTypes {
	None = 0x00,
	Geometry = 0x01,
	Control = 0x02,
	Evaluation = 0x04
};

class Shader {
	
public:
	unsigned int ID;
	Shader() = default;
	//Shader with option for each path
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr, const char* tessCtrlPath = nullptr, const char* tessEvalPath = nullptr);
	//Easier way to make shaders. Just pass a folder and the types of available shaders and ensure a file naming standard.
	Shader(std::string location, unsigned int shaderTypes);

	void init(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr, const char* tessCtrlPath = nullptr, const char* tessEvalPath = nullptr);
	
	virtual void use();
	// utility uniform functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec2(const std::string &name, float x, float y) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setVec4(const std::string &name, float x, float y, float z, float w);
	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
	void setMVP(glm::mat4 m, glm::mat4 v, glm::mat4 p);
protected:
	void checkCompileErrors(GLuint shader, std::string type);
	std::string shaderCodeString(const char* path);
	unsigned int shaderCode(std::string codeIn, std::string name, int type);
	unsigned int heightMap;
	unsigned int textures[8];
	std::string shaderName;
};

#endif