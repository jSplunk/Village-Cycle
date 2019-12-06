#include "Cube.h"



Cube::Cube()
{
	this->cubeVAO = 0;
	this->NormalVBO = 0;
	this->PositionVBO = 0;
	this->ColourVBO = 0;
	this->TexCoordVBO = 0;
	this->numElements = 0;
	this->IndexVBO = 0;
	this->VertexOffsetVBO = 0;
}

//size = size of the cube
Cube::Cube(float size)
{
	if (size <= 0) size = 1.0f;
	// Per-vertex position vectors
	float cubeVertices[32] =
	{
		-1.0f * size, 1.0f * size, -1.0f * size, 1.0f,
		-1.0f* size, 1.0f * size, 1.0f * size, 1.0f,
		1.0f * size, 1.0f * size, 1.0f * size, 1.0f,
		1.0f * size, 1.0f * size, -1.0f * size, 1.0f,
		-1.0f * size, -1.0f * size, -1.0f * size, 1.0f,
		-1.0f * size, -1.0f * size, 1.0f * size, 1.0f,
		1.0f * size, -1.0f * size, 1.0f * size, 1.0f,
		1.0f * size, -1.0f * size, -1.0f * size, 1.0f
	};

	float cubeNormals[24] =
	{
		-1.0f , 1.0f , -1.0f,
		-1.0f, 1.0f , 1.0f,
		1.0f , 1.0f, 1.0f,
		1.0f, 1.0f , -1.0f, 
		-1.0f , -1.0f, -1.0f, 
		-1.0f , -1.0f, 1.0f,
		1.0f , -1.0f , 1.0f,
		1.0f , -1.0f , -1.0f
	};


	// Per-vertex position vectors
	float cubeTexCoord[16] =
	{
		1.0f,0.0f,
		1.0f,1.0f,
		0.0f,1.0f,
		0.0f,0.0f,
		0.0f,0.0f,
		1.0f,0.0f,
		1.0f,1.0f,
		0.0f,1.0f 
	};

	// Per-vertex colours (RGBA) floating point values
	float cubeColours[32];

	int rows = 0;
	for (int i = 1; i < 33; i++)
	{
		if (i % 4 == 0)
		{
			cubeColours[i-1] = 1.0f; //Every fourth element is 1.0f
			rows++;
		}
		else
		{
			cubeColours[rows * 4 + ((i % 4) - 1)] = 0.5f; //RGB values are grey colour
		}
		
	}
	// 12 faces each with 3 vertices (each face forms a triangle) (36 indices total)
	unsigned short cubeVertexIndices[36] =
	{
		0, 1, 2, // top (+y)
		0, 2, 3,
		4, 7, 5, // bottom (-y)
		5, 7, 6,
		0, 4, 1, // -x
		1, 4, 5,
		2, 7, 3, // +x
		2, 6, 7,
		0, 3, 4, // -z
		3, 7, 4,
		1, 5, 2, // +z
		2, 5, 6
	};

	// Setup VAO for cube object
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	// Setup VBO for vertex position data
	glGenBuffers(1, &PositionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, PositionVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr); // attribute 0 gets data from bound VBO (so assign vertex position buffer to attribute 0)

	//// Setup VBO for vertex colour data
	glGenBuffers(1, &ColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, ColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColours), cubeColours, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr); // attribute 1 gets colour data

		//// Setup VBO for vertex normal data
	glGenBuffers(1, &NormalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, NormalVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNormals), cubeNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr); // attribute 2 gets normal data

	//	// Setup VBO for vertex texture data
	glGenBuffers(1, &TexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, TexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexCoord), cubeTexCoord, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, nullptr); // attribute 1 gets texture data

	// Enable vertex position and colour attribute arrays
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	// Setup VBO for face index array
	glGenBuffers(1, &IndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeVertexIndices), cubeVertexIndices, GL_STATIC_DRAW);

	// Unbind pyramid VAO (or bind another VAO for another object / effect)
	// If we didn't do this, we may alter the bindings created above.
	glBindVertexArray(0);

	//Setting number of indices
	numElements = 36;

}

//size = size of the cube, radius = spawn area
Cube::Cube(float size, float radius)
{
	//Setting the amount of cubes to render
	Amount = radius / size;

	//Setting the offsets of the amount of cubes we want to render
	std::vector<glm::vec3> offsets;
	if (Amount > 1)
	{
		offsets.resize(Amount*Amount);

		float posX = radius - (size * 2);
		float posZ = radius - (size * 2);

		for (int i = 0; i < Amount; i++)
		{
			for (int j = 0; j < Amount; j++)
			{
				offsets[i * Amount + j].x = posX;
				offsets[i * Amount + j].z = posZ;

				posX -= (size * 2);
			}
			posZ -= (size * 2);
			posX = radius - (size * 2);
		}

		if (size <= 0) size = 1.0f;
	}
	else
	{
		offsets.resize(1);
		offsets[0].x = 0;
		offsets[0].z = 0;
		offsets[0].y = 0;
	}
	// Per-vertex position vectors
	float cubeVertices[32] =
	{
		-1.0f * size, 1.0f, -1.0f * size, 1.0f,
		-1.0f* size, 1.0f, 1.0f * size, 1.0f,
		1.0f * size, 1.0f , 1.0f * size, 1.0f,
		1.0f * size, 1.0f, -1.0f * size, 1.0f,
		-1.0f * size, -1.0f, -1.0f * size, 1.0f,
		-1.0f * size, -1.0f, 1.0f * size, 1.0f,
		1.0f * size, -1.0f, 1.0f * size, 1.0f,
		1.0f * size, -1.0f, -1.0f * size, 1.0f
	};

	float cubeNormals[24] =
	{
		-1.0f , 1.0f , -1.0f,
		-1.0f, 1.0f , 1.0f,
		1.0f , 1.0f, 1.0f,
		1.0f, 1.0f , -1.0f,
		-1.0f , -1.0f, -1.0f,
		-1.0f , -1.0f, 1.0f,
		1.0f , -1.0f , 1.0f,
		1.0f , -1.0f , -1.0f
	};


	// Per-vertex position vectors
	float cubeTexCoord[16] =
	{
		1.0f * size,0.0f,
		0.0f,0.0f,

		0.0f,1.0f * size,
		1.0f * size,1.0f * size,

		1.0f * size,0.0f,
		0.0f,0.0f,

		0.0f,1.0f * size,
		1.0f * size,1.0f * size,
	};

	// Per-vertex colours (RGBA) floating point values
	float cubeColours[32];

	int rows = 0;
	for (int i = 1; i < 33; i++)
	{
		if (i % 4 == 0)
		{
			cubeColours[i - 1] = 1.0f; //Every fourth element is 1.0f
			rows++;
		}
		else
		{
			cubeColours[rows * 4 + ((i % 4) - 1)] = 0.1f; //RGB values are grey colour
		}

	}

	// 12 faces each with 3 vertices (each face forms a triangle) (36 indices total)
	unsigned short cubeVertexIndices[36] =
	{
		0, 1, 2, // top (+y)
		0, 2, 3,
		4, 7, 5, // bottom (-y)
		5, 7, 6,
		0, 4, 1, // -x
		1, 4, 5,
		2, 7, 3, // +x
		2, 6, 7,
		0, 3, 4, // -z
		3, 7, 4,
		1, 5, 2, // +z
		2, 5, 6
	};

	// Setup VAO for cube object
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	// Setup VBO for vertex position data
	glGenBuffers(1, &PositionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, PositionVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr); // attribute 0 gets data from bound VBO (so assign vertex position buffer to attribute 0)

	//// Setup VBO for vertex colour data
	glGenBuffers(1, &ColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, ColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColours), cubeColours, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr); // attribute 1 gets colour data

		//// Setup VBO for vertex normal data
	glGenBuffers(1, &NormalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, NormalVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNormals), cubeNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr); // attribute 2 gets normal data

	//	// Setup VBO for vertex texture data
	glGenBuffers(1, &TexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, TexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexCoord), cubeTexCoord, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, nullptr); // attribute 1 gets texture data

	// Setup VBO for vertex instanced array
	glGenBuffers(1, &VertexOffsetVBO);
	glBindBuffer(GL_ARRAY_BUFFER, VertexOffsetVBO);
	if(Amount > 1)
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Amount * Amount, glm::value_ptr(offsets[0]), GL_STREAM_DRAW);
	else
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), glm::value_ptr(offsets[0]), GL_STREAM_DRAW);
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glVertexAttribDivisor(6, 1);



	// Enable vertex position and colour attribute arrays
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(6);

	// Setup VBO for face index array
	glGenBuffers(1, &IndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeVertexIndices), cubeVertexIndices, GL_STATIC_DRAW);

	// Unbind pyramid VAO (or bind another VAO for another object / effect)
	// If we didn't do this, we may alter the bindings created above.
	glBindVertexArray(0);

	//Setting number of indices
	numElements = 36;

}

Cube::Cube(float width, float breadth, float radius)
{

	float size = (width*breadth);

	//Setting how many cubes we are rendering
	Amount = radius / size;

	//Setting the offsets for the amount cube we are rendering
	std::vector<glm::vec3> offsets;

	if (Amount > 1)
	{
		
		offsets.resize(Amount*Amount);

		float posX = radius - (width * 2);
		float posZ = radius - (breadth * 2);

		for (int i = 0; i < Amount; i++)
		{
			for (int j = 0; j < Amount; j++)
			{
				offsets[i * Amount + j].x = posX;
				offsets[i * Amount + j].z = posZ;

				posX -= (width * 2);
			}
			posZ -= (breadth * 2);
			posX = radius - (width * 2);
		}

		if (width <= 0) width = 1.0f;
		if (breadth <= 0) breadth = 1.0f;
	}
	else
	{
		offsets.resize(1);
		offsets[0].x = 0;
		offsets[0].z = 0;
		offsets[0].y = 0;
	}

	// Per-vertex position vectors
	float cubeVertices[32] =
	{
		-1.0f * width, 1.0f, -1.0f * breadth, 1.0f,
		-1.0f* width, 1.0f, 1.0f * breadth, 1.0f,
		1.0f * width, 1.0f, 1.0f * breadth, 1.0f,
		1.0f * width, 1.0f, -1.0f * breadth, 1.0f,
		-1.0f * width, -1.0f, -1.0f * breadth, 1.0f,
		-1.0f * width, -1.0f, 1.0f * breadth, 1.0f,
		1.0f * width, -1.0f, 1.0f * breadth, 1.0f,
		1.0f * width, -1.0f, -1.0f * breadth, 1.0f
	};

	float cubeNormals[24] =
	{
		-1.0f , 1.0f , -1.0f,
		-1.0f, 1.0f , 1.0f,
		1.0f , 1.0f, 1.0f,
		1.0f, 1.0f , -1.0f,
		-1.0f , -1.0f, -1.0f,
		-1.0f , -1.0f, 1.0f,
		1.0f , -1.0f , 1.0f,
		1.0f , -1.0f , -1.0f
	};


	// Per-vertex position vectors
	float cubeTexCoord[16] =
	{
		1.0f * width,0.0f,
		0.0f,0.0f,

		0.0f,1.0f * breadth,
		1.0f * width,1.0f * breadth,

		1.0f * width,0.0f,
		0.0f,0.0f,

		0.0f,1.0f * breadth,
		1.0f * width,1.0f * breadth,
	};

	// Per-vertex colours (RGBA) floating point values
	float cubeColours[32];

	//Setting colours
	int rows = 0;
	for (int i = 1; i < 33; i++)
	{
		if (i % 4 == 0)
		{
			cubeColours[i - 1] = 1.0f; //Every fourth element is 1.0f
			rows++;
		}
		else
		{
			cubeColours[rows * 4 + ((i % 4) - 1)] = 0.1f; //RGB values are grey colour
		}

	}

	// 12 faces each with 3 vertices (each face forms a triangle) (36 indices total)
	unsigned short cubeVertexIndices[36] =
	{
		0, 1, 2, // top (+y)
		0, 2, 3,
		4, 7, 5, // bottom (-y)
		5, 7, 6,
		0, 4, 1, // -x
		1, 4, 5,
		2, 7, 3, // +x
		2, 6, 7,
		0, 3, 4, // -z
		3, 7, 4,
		1, 5, 2, // +z
		2, 5, 6
	};

	// Setup VAO for cube object
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	// Setup VBO for vertex position data
	glGenBuffers(1, &PositionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, PositionVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr); // attribute 0 gets data from bound VBO (so assign vertex position buffer to attribute 0)

	//// Setup VBO for vertex colour data
	glGenBuffers(1, &ColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, ColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColours), cubeColours, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr); // attribute 1 gets colour data

		//// Setup VBO for vertex normal data
	glGenBuffers(1, &NormalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, NormalVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNormals), cubeNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr); // attribute 2 gets normal data

	//	// Setup VBO for vertex texture data
	glGenBuffers(1, &TexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, TexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexCoord), cubeTexCoord, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, nullptr); // attribute 1 gets texture data

	// Setup VBO for vertex instanced array
	glGenBuffers(1, &VertexOffsetVBO);
	glBindBuffer(GL_ARRAY_BUFFER, VertexOffsetVBO);
	if (Amount > 1)
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Amount * Amount, glm::value_ptr(offsets[0]), GL_STREAM_DRAW);
	else
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), glm::value_ptr(offsets[0]), GL_STREAM_DRAW);
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glVertexAttribDivisor(6, 1);



	// Enable vertex position and colour attribute arrays
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(6);

	// Setup VBO for face index array
	glGenBuffers(1, &IndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeVertexIndices), cubeVertexIndices, GL_STREAM_DRAW);

	// Unbind VAO (or bind another VAO for another object / effect)
	// If we didn't do this, we may alter the bindings created above.
	glBindVertexArray(0);

	//Setting number of indices
	numElements = 36;

}

void Cube::render()
{

	glBindVertexArray(cubeVAO);

	//Checking to see if we are rendering more than one cube
	if(Amount > 1)
		glDrawElementsInstanced(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, (const GLvoid*)0, Amount*Amount);
	else
		glDrawElementsInstanced(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, (const GLvoid*)0, 1);
	glBindVertexArray(0);
}

Cube::~Cube()
{
}
