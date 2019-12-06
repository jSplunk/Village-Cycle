#pragma once
#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Camera.h"
#include "Shader.h"

class HDR
{
private:
	//Local version of camera setting passed into the HDR class for access to screen width and screen height
	Camera_settings settings; 

	Shader hdrShader = { "Resources\\Shaders\\HDR_shader.vs", "Resources\\Shaders\\HDR_shader.fs" };

	//Buffers for FBO, RBO, VAO and VBO
	unsigned int hdrFBO;
	unsigned int fboColorBuffer;
	unsigned int rboDepth;
	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	//Function to render a textured quad with our HDR color buffer
	void renderQuad();

public:

	//Public contructor
	HDR(Camera_settings settings);

	//Function for binding the framebuffer
	void bindHDRFramebuffer();

	//Function for un binding the framebuffer
	void unBindHDRFramebuffer();

	//Function to render the HDR scene
	bool render(Camera, unsigned int, unsigned int,bool, float, bool);
	~HDR();
};

