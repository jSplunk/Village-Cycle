#pragma once
#include "Shader.h"
#include "Camera.h"
#include <iostream>

class GaussianBlur
{
private:

	GLuint hdrFBO;
	// create 2 floating point color buffers (1 for normal rendering, other for brightness treshold values)
	GLuint colourBuffers[2];

	// create and attach depth buffer (renderbuffer)
	GLuint rboDepth;
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

	// Gaussian framebuffer for blurring
	GLuint gaussianFBOs[2];
	// Gaussian colourbuffer for blurring
	GLuint gaussianColourbuffers[2];

	//Shader
	Shader gaussianBlurShader = { "Resources\\Shaders\\Gaussian_shader.vs","Resources\\Shaders\\Gaussian_shader.fs" };
	Shader gaussianFinalBlurShader = { "Resources\\Shaders\\FinalBlur_shader.vs","Resources\\Shaders\\FinalBlur_shader.fs" };

	Camera_settings settings;


	GLuint quadVAO = 0;
	GLuint quadVBO;

	void renderQuad();

	bool horizontal = true;
	

public:
	GaussianBlur(Camera_settings);
	void gaussianBlur();
	void render(Camera&, bool&, float&);
	void bindFrameBuffer();
	void unBindFrameBuffer();
};