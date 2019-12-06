#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
using namespace std;

struct Vertex {
	// position
	glm::vec3 Position;
	// colour
	glm::vec3 Colour;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;
	string type;
	string name;
	string path;
};

class Mesh {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	unsigned int VAO;

	// offsets
	vector<glm::vec3> Offsets;
	int amountToSpawn;

	/*  Functions  */
	// constructor
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, vector<glm::vec3> Offsets, int amountToSpawn);

	// render the mesh
	void draw(GLuint shader);
	void insertTexture(Texture);

private:
	/*  Render data  */
	unsigned int VBO, EBO, InstancedOffsets;

	// initializes all the buffer objects/arrays
	void setupMesh();

};
#endif

