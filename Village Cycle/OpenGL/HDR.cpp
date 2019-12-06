#include "HDR.h"



HDR::HDR(Camera_settings settings)
{
	this->settings = settings;

	// configure floating point framebuffer
	// ------------------------------------
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	
	// create floating point color buffer
	glGenTextures(1, &fboColorBuffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, fboColorBuffer);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA16F, settings.screenWidth*2, settings.screenHeight*2, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	// create depth buffer (renderbuffer)
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, settings.screenWidth*2, settings.screenHeight*2);
	glBindRenderbuffer(GL_FRAMEBUFFER, 0);

	//Bind the fbo colour buffer to the hdr fbo
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, fboColorBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Bind shader locations and their values
	hdrShader.use();
	hdrShader.setInt("hdrBuffer", 0);
	hdrShader.setInt("vpWidth", settings.screenWidth);
	hdrShader.setInt("vpHeight", settings.screenHeight);
}

void HDR::bindHDRFramebuffer()
{
	//Bind the frambuffer
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
}

void HDR::unBindHDRFramebuffer()
{
	//Un bind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool HDR::render(Camera cam, unsigned int width, unsigned int height, bool hdr, float exposure, bool greyscale)
{
	// 2. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
	// --------------------------------------------------------------------------------------------------------------------------
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Setting our viewport to the screen width and screen height

	glViewport(0, 0, width, height);

	glm::mat4 view = cam.getViewMatrix();
	glm::mat4 projection = cam.getProjectionMatrix();

	//Use the HDR shader
	hdrShader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, fboColorBuffer);

	//Set the uniform locations and their values
	hdrShader.setMat4("mvpMatrix", projection * view);
	hdrShader.setInt("hdr", hdr);
	hdrShader.setFloat("exposure", exposure);
	hdrShader.setInt("greyScale", greyscale);
	renderQuad();
	return true;
}


void HDR::renderQuad()
{
	//As long as we have initialized quadVAO we render
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STREAM_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

HDR::~HDR()
{
	//Un bind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
