#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <random>
class Cube
{
private:
	//Cube VBOs
	GLuint	PositionVBO;
	GLuint	ColourVBO;
	GLuint	NormalVBO;
	GLuint	TexCoordVBO;
	GLuint	IndexVBO;
	GLuint  VertexOffsetVBO;

	//Cube VAO
	GLuint	cubeVAO;

	//Number of indices in the cude
	GLuint	numElements;

	//Amount of cubes to render
	float	Amount;
public:
	
	//Contructors
	Cube();
	Cube(float);
	Cube(float, float);
	Cube(float, float, float);

	//Render the cube
	void render();
	~Cube();
};

