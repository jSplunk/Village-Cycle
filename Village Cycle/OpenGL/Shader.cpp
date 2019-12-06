#include "Shader.h"

Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath)
{
	//Loading shader
	GLSL_ERROR glsl_err = ShaderLoader::createShaderProgram(
		std::string(vertexShaderPath),
		std::string(fragmentShaderPath),
		&id);
	if (glsl_err) id = -1;
}

Shader::~Shader()
{
	glUseProgram(0);
}

void Shader::use()
{
	//Using shader
	glUseProgram(id);
}

//Setting uniforms
//----------------------------------------------------------------------------------
void Shader::setInt(const std::string& location, int x)
{
	glUniform1i(glGetUniformLocation(id, location.c_str()), x);
}

void Shader::setFloat(const std::string& location, float x)
{
	glUniform1f(glGetUniformLocation(id, location.c_str()), x);
}

void Shader::setFloat2(const std::string& location, float x, float y)
{
	glUniform2f(glGetUniformLocation(id, location.c_str()), x, y);
}

void Shader::setFloat3(const std::string& location, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(id, location.c_str()), x, y, z);
}

void Shader::setFloat4(const std::string& location, float x,float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(id, location.c_str()), x, y, z, w);
}

void Shader::setVec2(const std::string& location, glm::vec2& vec)
{
	glUniform3fv(glGetUniformLocation(id, location.c_str()), 1, &vec[0]);
}

void Shader::setVec3(const std::string& location, glm::vec3& vec)
{
	glUniform3fv(glGetUniformLocation(id, location.c_str()), 1, &vec[0]);
}

void Shader::setVec4(const std::string& location, glm::vec4& vec)
{
	glUniform3fv(glGetUniformLocation(id, location.c_str()), 1, &vec[0]);
}

void Shader::setMat4(const std::string& location, glm::mat4& mat)
{
	glUniformMatrix4fv(glGetUniformLocation(id, location.c_str()), 1, false, &mat[0][0]);
}
//-------------------------------------------------------------------------------------------