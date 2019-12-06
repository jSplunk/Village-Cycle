#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

#include "ShaderLoader.h"


class Shader
{
private:
	GLuint id;

public:
	Shader(std::string vertexShaderPath, std::string fragmentShaderPath);
	~Shader();
	//Function to use shader
	void use();

	//Get the ID of the shader
	inline GLuint getShaderId() const { return id; }

	//Setting uniforms
	void setInt(const std::string&, int);
	void setFloat(const std::string&, float);
	void setFloat2(const std::string&, float, float);
	void setFloat3(const std::string&, float, float, float);
	void setFloat4(const std::string&, float, float, float, float);
	void setVec2(const std::string&, glm::vec2&);
	void setVec3(const std::string&, glm::vec3&);
	void setVec4(const std::string&, glm::vec4&);
	void setMat4(const std::string&, glm::mat4&);
};

