#include <shader.h>

#include "texture/stb_image.h"

Shader::Shader(std::string location, unsigned int shaderTypes) {
	shaderName = location;
	std::string vert = "..\\shaders\\" + location + "\\vertex.vert";
	std::string frag = "..\\shaders\\" + location + "\\fragment.frag";
	std::string geom = "..\\shaders\\" + location + "\\geometry.geom"; //1
	std::string tesc = "..\\shaders\\" + location + "\\control.tesc"; //2
	std::string tese = "..\\shaders\\" + location + "\\evaluation.tese"; //4
	
	init(
		vert.c_str(),
		frag.c_str(),
		shaderTypes & Geometry ? geom.c_str() : nullptr,
		shaderTypes & Control ? tesc.c_str() : nullptr,
		shaderTypes & Evaluation ? tese.c_str() : nullptr
	);
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath, const char* tessCtrlPath, const char* tessEvalPath) {
	shaderName = "Unnamed";
	init(vertexPath, fragmentPath, geometryPath, tessCtrlPath, tessEvalPath);
}

void Shader::init(const char* vertexPath, const char* fragmentPath, const char* geometryPath, const char* tessCtrlPath, const char* tessEvalPath) {
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::string tessCtrlCode;
	std::string tessEvalCode;

	try	{
		vertexCode = shaderCodeString(vertexPath);
		fragmentCode = shaderCodeString(fragmentPath);

		if (geometryPath != nullptr) geometryCode = shaderCodeString(geometryPath);
		if (tessCtrlPath != nullptr) tessCtrlCode = shaderCodeString(tessCtrlPath);
		if (tessEvalPath != nullptr) tessEvalCode = shaderCodeString(tessEvalPath);
	} catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	
	// 2. compile shaders
	unsigned int vertex   = shaderCode(vertexCode,   "VERTEX", GL_VERTEX_SHADER);
	unsigned int fragment = shaderCode(fragmentCode, "FRAGMENT", GL_FRAGMENT_SHADER);
	unsigned int geometry = shaderCode(geometryCode, "GEOMETRY", GL_GEOMETRY_SHADER);
	unsigned int tessCtrl = shaderCode(tessCtrlCode, "TESS CONTROL", GL_TESS_CONTROL_SHADER);
	unsigned int tessEval = shaderCode(tessEvalCode, "TESS EVALUATION", GL_TESS_EVALUATION_SHADER);
	
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	if (geometryPath != nullptr) glAttachShader(ID, geometry);
	if (tessCtrlPath != nullptr) glAttachShader(ID, tessCtrl);
	if (tessEvalPath != nullptr) glAttachShader(ID, tessEval);
	
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr) glDeleteShader(geometry);
	if (tessCtrlPath != nullptr) glDeleteShader(tessCtrl);
	if (tessEvalPath != nullptr) glDeleteShader(tessEval);

	std::cout << "Loading " << shaderName << " complete." << std::endl << std::endl;
}

std::string Shader::shaderCodeString(const char* path) {
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	shaderFile.open(path);
	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	shaderFile.close();
	return shaderStream.str();
}

unsigned int Shader::shaderCode(std::string codeIn, std::string name, int type) {
	if (codeIn.length() > 5) {
		const char* shaderCode = codeIn.c_str();
		unsigned int shader = glCreateShader(type);
		glShaderSource(shader, 1, &shaderCode, NULL);
		glCompileShader(shader);
		checkCompileErrors(shader, name);
		std::cout << " loaded " << shaderName << " " << name << std::endl;
		return shader;
	}
	
	return -1;
}

void Shader::setMVP(glm::mat4 m, glm::mat4 v, glm::mat4 p) {
	setMat4("projection", p);
	setMat4("view", v);
	setMat4("model", m);
}

//compute shader stuff
void stuff() {
	glDispatchCompute(32,16,1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	//texMan->drawTexture(output_img);

}
void writeImage() {
	//pixel not UV: uv*texelSize
	//use rgba not rgb
	//glBindImageTexture();
}

#pragma region PROVIDED_CODE
void Shader::use()
{
	glUseProgram(ID);
}

// ------------------------------------------------------------------------
void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << shaderName << ": ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << shaderName << ": ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
#pragma endregion

