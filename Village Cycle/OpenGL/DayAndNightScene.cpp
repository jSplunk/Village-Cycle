#include "DayAndNightScene.h"

DayAndNightScene::DayAndNightScene(Camera_settings settings)
{
	//Save the camera settings into a local variable
	this->settings = settings;

	// Create Earth sphere model
	earthModel = new Sphere(8, 8, 1.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), CG_RIGHTHANDED);
	cube = new Cube(100.0f, 1);

	//Generate texture properties
	TextureGenProperties prop(GL_SRGB8_ALPHA8);
	prop.genMipMaps = false;
	prop.maxFilter = GL_LINEAR_MIPMAP_LINEAR;
	prop.minFilter = GL_LINEAR_MIPMAP_LINEAR;

	//Textures
	//demoTexture = TextureLoader::loadTexture(string("Resources\\Models\\nightAndDay.png")); //GL_SRGB8_ALPHA8
	grassTexture = TextureLoader::loadTexture(string("Resources\\Models\\grass2.png"), prop); //GL_SRGB8_ALPHA8
	grassTexture2 = TextureLoader::loadTexture(string("Resources\\Models\\plants_lowhigh\\Low\\Grass.png")); //GL_SRGB8_ALPHA8

	//Attaching the grass textures to the grass model
	grass.attachTexture(grassTexture, "dayTexture");
	grass.attachTexture(grassTexture, "nightTexture");

	//Setting the texture uniforms in the earth shader
	earth.use();
	earth.setInt("dayTexture", 0);
	earth.setInt("nightTexture", 1);
}

DayAndNightScene::~DayAndNightScene()
{
	//Cleans up our pointers to objects
	delete cube;
	delete earthModel;
}

void DayAndNightScene::render(Camera camera, Timer timer, float speedUpTime)
{
	// Clear the screen
	glClearColor(((cX * 255) - (255 - 83)) / 255, ((cY * 255) - (255 - 131)) / 255, ((cZ * 255) - (255 - 171)) / 255, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, settings.screenWidth, settings.screenHeight);

	//glm::mat4 model = glm::scale(glm::mat4(1.0), glm::vec3(0.28, 0.28, 0.28));
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 projection = camera.getProjectionMatrix();
	glm::mat4 viewProjection = projection * view;
	glm::vec3 cameraPos = camera.getCameraPosition();
	//principleAxes->render(viewProjection);


	if (earthModel && cube)
	{
		// Modelling transform for sun and moon objects
		glm::mat4 modelTransform = glm::scale(glm::mat4(1.0), glm::vec3(3.0f));
		glm::mat4 modelTransform2 = glm::scale(glm::mat4(1.0), glm::vec3(3.0f));
		modelTransform = glm::translate(modelTransform, glm::vec3(0, 0, 0));
		modelTransform2 = glm::translate(modelTransform2, glm::vec3(0, 0, 0));
		modelTransform = glm::rotate(modelTransform, glm::radians(earthTheta), glm::vec3(0.0, 0.0, -1.0));//Earth rotation
		modelTransform2 = glm::rotate(modelTransform2, glm::radians(earthTheta), glm::vec3(0.0, 0.0, -1.0));//Earth rotation
		modelTransform = glm::translate(modelTransform, glm::vec3(0, 20, -20));
		modelTransform2 = glm::translate(modelTransform2, glm::vec3(0, -20, -20));

		//Drawing the sun and the moon objects
		sunAndMoon.use();
		sunAndMoon.setVec3("cameraPos", cameraPos);
		sunAndMoon.setMat4("modelMatrix", modelTransform);
		sunAndMoon.setMat4("viewProjectionMatrix", viewProjection);
		sunAndMoon.setFloat4("lightDirection", 0.0f, 1.0f, 0.0f, 0.0f);
		sunAndMoon.setFloat4("lightDiffuseColour", 0.0f, 0.0f, 0.0f, 1.0f);
		sunAndMoon.setFloat4("lightAmbientColour", 1.0f, 1.0f, 0.0f, 1.0f);
		sunAndMoon.setFloat4("lightSpecularColour", 1.0f, 1.0f, 1.0f, 1.0f);
		sunAndMoon.setFloat("lightSpecularExponent", 1.0f);

		// Setup uniform locations for shader
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);

		earthModel->render();

		sunAndMoon.setMat4("modelMatrix", modelTransform2);
		sunAndMoon.setFloat4("lightAmbientColour", 1.0f, 1.0f, 1.0f, 1.0f);

		earthModel->render();


		//Drawing grass
		//Model transform for grass
		modelTransform = glm::scale(glm::mat4(1.0), glm::vec3(0.5f));
		modelTransform = glm::translate(modelTransform, glm::vec3(0, 1.0f, 0));

		// Calculate inverse transpose of the modelling transform for correct transformation of normal vectors
		glm::mat4 inverseTranspose = glm::transpose(glm::inverse(glm::rotate(modelTransform, glm::radians(earthTheta), glm::vec3(0.0, 0.0, 1.0))));
		
		earth.use();
		earth.setVec3("cameraPos", cameraPos);
		earth.setMat4("modelMatrix", modelTransform);
		earth.setMat4("viewProjectionMatrix", viewProjection);
		earth.setMat4("invTransposeModelMatrix", inverseTranspose);
		earth.setFloat4("lightDirection", 0.0f, 1.0f, -1.0f, 0.0f);
		earth.setFloat4("lightDiffuseColour", 0.24f, 0.75f, 0.25f, 1.0f);
		earth.setFloat4("lightAmbientColour", 0.0f, 0.0f, 0.0f, 1.0f);
		earth.setFloat4("lightSpecularColour", 0.0f, 0.0f, 0.0f, 1.0f);
		earth.setFloat("lightSpecularExponent", 1.0f);

		grass.draw(earth.getShaderId());


		//Drawing houses
		//Model transforms for houses
		modelTransform = glm::scale(glm::mat4(1.0), glm::vec3(0.5f));
		modelTransform = glm::translate(modelTransform, glm::vec3(0, 1.0f, 0));
		inverseTranspose = glm::transpose(glm::inverse(glm::rotate(modelTransform, glm::radians(earthTheta), glm::vec3(0.0, 0.0, 1.0))));

		earth.setFloat4("lightDiffuseColour", 0.37f, 0.15f, 0.01f, 1.0f);
		earth.setMat4("modelMatrix", modelTransform);
		earth.setMat4("viewProjectionMatrix", viewProjection);
		earth.setMat4("invTransposeModelMatrix", inverseTranspose);

		house.draw(earth.getShaderId());


		//Drawing another set of houses
		//Model transforms for houses
		modelTransform = glm::scale(glm::mat4(1.0), glm::vec3(0.5f));
		modelTransform = glm::translate(modelTransform, glm::vec3(0, 1.0f, 0));
		inverseTranspose = glm::transpose(glm::inverse(glm::rotate(modelTransform, glm::radians(earthTheta), glm::vec3(0.0, 0.0, 1.0))));

		earth.setFloat4("lightDiffuseColour", 0.37f, 0.15f, 0.01f, 1.0f);
		earth.setMat4("modelMatrix", modelTransform);
		earth.setMat4("viewProjectionMatrix", viewProjection);
		earth.setMat4("invTransposeModelMatrix", inverseTranspose);

		house2.draw(earth.getShaderId());


		//Drawing bushes
		//Model transforms for bushes
		modelTransform = glm::scale(glm::mat4(1.0), glm::vec3(0.3f));
		modelTransform = glm::translate(modelTransform, glm::vec3(0, 1.0f, 0));
		inverseTranspose = glm::transpose(glm::inverse(glm::rotate(modelTransform, glm::radians(earthTheta), glm::vec3(0.0, 0.0, 1.0))));

		earth.setFloat4("lightDiffuseColour", 0.24f, 0.75f, 0.25f, 1.0f);
		earth.setMat4("modelMatrix", modelTransform);
		earth.setMat4("viewProjectionMatrix", viewProjection);
		earth.setMat4("invTransposeModelMatrix", inverseTranspose);

		bush.draw(earth.getShaderId());


		//Drawing rocks
		//Model transforms for rocks
		modelTransform = glm::scale(glm::mat4(1.0), glm::vec3(0.3f));
		modelTransform = glm::translate(modelTransform, glm::vec3(0, 2.0f, 0));
		inverseTranspose = glm::transpose(glm::inverse(glm::rotate(modelTransform, glm::radians(earthTheta), glm::vec3(0.0, 0.0, 1.0))));

		earth.setFloat4("lightDiffuseColour", 0.1f, 0.1f, 0.1f, 1.0f);
		earth.setFloat4("lightAmbientColour", 0.04f, 0.04f, 0.04f, 1.0f);
		earth.setMat4("modelMatrix", modelTransform);
		earth.setMat4("viewProjectionMatrix", viewProjection);
		earth.setMat4("invTransposeModelMatrix", inverseTranspose);

		rock.draw(earth.getShaderId());


		//Drawing the terrain
		//Model transforms for terrain
		modelTransform = glm::scale(glm::mat4(1.0), glm::vec3(1.0f));
		inverseTranspose = glm::transpose(glm::inverse(glm::rotate(modelTransform, glm::radians(earthTheta), glm::vec3(0.0, 0.0, 1.0))));

		earth.setInt("dayTexture", 0);
		earth.setInt("nightTexture", 1);
		earth.setMat4("modelMatrix", modelTransform);
		earth.setMat4("viewProjectionMatrix", viewProjection);
		earth.setMat4("invTransposeModelMatrix", inverseTranspose);
		earth.setFloat4("lightDirection", 0.0f, 1.0f, 0.0f, 0.0f);
		earth.setFloat4("lightDiffuseColour", 0.5f, 0.5f, 0.5f, 1.0f);
		earth.setFloat4("lightAmbientColour", 0.0f, 0.0f, 0.0f, 1.0f);
		earth.setFloat4("lightSpecularColour", 0.0f, 0.0f, 0.0f, 1.0f);
		earth.setFloat("lightSpecularExponent", 1.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, grassTexture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, grassTexture);

		cube->render();
	}

	//Controls the change of rotation of the light around the scene
	earthTheta += float(timer.getDeltaTimeSeconds()) * 6 * speedUpTime / minutesADay;

	//Controls the change of colours displayed in the background
	change += (float(timer.getDeltaTimeSeconds())/ 2.0f) * 6 * speedUpTime / minutesADay;

	//Calculates a value between -1 and 1 for all colour attributes x,y and z
	cX = (cosf(glm::radians(change)));
	cY = (cosf(glm::radians(change)));
	cZ = (cosf(glm::radians(change)));

	//Cheking if the colour value is below a certain value, in our case its a dark colour
	//If we get that colour we have the background stay at that colour,
	//Otherwise we continue to change it according to the change variable
	if (cosf(glm::radians(change)) < 0.6f) cX = 0.6f;
	if (cosf(glm::radians(change)) < -0.6f) cX = cosf(glm::radians(change)) * -1;
	if (cosf(glm::radians(change)) < 0.6f) cY = 0.6f;
	if (cosf(glm::radians(change)) < -0.6f) cY = cosf(glm::radians(change)) * -1;
	if (cosf(glm::radians(change)) < 0.6f) cZ = 0.6f;
	if (cosf(glm::radians(change)) < -0.6f) cZ = cosf(glm::radians(change)) * -1;
	
}

void DayAndNightScene::renderLights(Camera camera, Timer timer, float speedUpTime)
{
	// Clear the screen
	glClearColor(((cX * 255) - (255 - 83)) / 255, ((cY * 255) - (255 - 131)) / 255, ((cZ * 255) - (255 - 171)) / 255, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glViewport(0, 0, settings.screenWidth, settings.screenHeight);

	glm::mat4 model = glm::scale(glm::mat4(1.0), glm::vec3(0.28, 0.28, 0.28));
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 projection = camera.getProjectionMatrix();
	glm::mat4 viewProjection = projection * view;
	glm::vec3 cameraPos = camera.getCameraPosition();
	//principleAxes->render(viewProjection);


	if (earthModel)
	{
		// Modelling transform

		glm::mat4 modelTransform = glm::scale(glm::mat4(1.0), glm::vec3(3.0f));
		glm::mat4 modelTransform2 = glm::scale(glm::mat4(1.0), glm::vec3(3.0f));
		modelTransform = glm::translate(modelTransform, glm::vec3(0, 0, 0));
		modelTransform2 = glm::translate(modelTransform2, glm::vec3(0, 0, 0));
		modelTransform = glm::rotate(modelTransform, glm::radians(earthTheta), glm::vec3(0.0, 0.0, -1.0));//Earth rotation
		modelTransform2 = glm::rotate(modelTransform2, glm::radians(earthTheta), glm::vec3(0.0, 0.0, -1.0));//Earth rotation
		modelTransform = glm::translate(modelTransform, glm::vec3(0, 20, -20));
		modelTransform2 = glm::translate(modelTransform2, glm::vec3(0, -20, -20));

		// Calculate inverse transpose of the modelling transform for correct transformation of normal vectors
		glm::mat4 inverseTranspose = glm::transpose(glm::inverse(modelTransform));
		//Setup uniform locations for shader


		sunAndMoon.use();

		sunAndMoon.setMat4("modelMatrix", modelTransform);
		sunAndMoon.setMat4("viewProjectionMatrix", viewProjection);

		sunAndMoon.setFloat4("lightDiffuseColour", 0.0f, 0.0f, 0.0f, 1.0f);
		sunAndMoon.setFloat4("lightAmbientColour", 1.0f, 1.0f, 0.0f, 1.0f);

		// Setup uniform locations for shader

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);

		earthModel->render();

		sunAndMoon.setMat4("modelMatrix", modelTransform2);
		sunAndMoon.setFloat4("lightAmbientColour", 1.0f, 1.0f, 1.0f, 1.0f);

		earthModel->render();





		/*modelTransform = glm::scale(glm::mat4(1.0), glm::vec3(1.0f));
		modelTransform = glm::translate(modelTransform, glm::vec3(0, 0.5, 0));

		inverseTranspose = glm::transpose(glm::rotate(modelTransform, glm::radians(earthTheta - 90.0f), glm::vec3(0.0, 0.0, 1.0)));*/

		/*sunAndMoon.setInt("dayTexture", 0);
		sunAndMoon.setInt("nightTexture", 1);

		sunAndMoon.setVec3("cameraPos", cameraPos);

		sunAndMoon.setMat4("modelMatrix", modelTransform);
		sunAndMoon.setMat4("viewProjectionMatrix", viewProjection);
		sunAndMoon.setMat4("invTransposeModelMatrix", inverseTranspose);

		sunAndMoon.setFloat4("lightDirection", 0.0f, 1.0f, 0.0f, 0.0f);
		sunAndMoon.setFloat4("lightDiffuseColour", 0.0f, 0.0f, 0.0f, 1.0f);
		sunAndMoon.setFloat4("lightAmbientColour", 0.0f, 0.0f, 0.0f, 1.0f);
		sunAndMoon.setFloat4("lightSpecularColour", 0.0f, 0.0f, 0.0f, 1.0f);

		sunAndMoon.setFloat("lightSpecularExponent", 0.0f);

		grass.draw(sunAndMoon.getShaderId());*/


		/*modelTransform = glm::scale(glm::mat4(1.0), glm::vec3(1.0f));
		inverseTranspose = glm::transpose(glm::inverse(glm::rotate(modelTransform, glm::radians(earthTheta), glm::vec3(0.0, 0.0, 1.0))));

		sunAndMoon.setMat4("modelMatrix", modelTransform);
		sunAndMoon.setMat4("viewProjectionMatrix", viewProjection);
		sunAndMoon.setMat4("invTransposeModelMatrix", inverseTranspose);

		sunAndMoon.setFloat4("lightDirection", 0.0f, 1.0f, 0.0f, 0.0f);
		sunAndMoon.setFloat4("lightDiffuseColour", 0.0f, 0.0f, 0.0f, 1.0f);
		sunAndMoon.setFloat4("lightAmbientColour", 0.0f, 0.0f, 0.0f, 1.0f);
		sunAndMoon.setFloat4("lightSpecularColour", 0.0f, 0.0f, 0.0f, 1.0f);

		sunAndMoon.setFloat("lightSpecularExponent", 0.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, grassTexture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, grassTexture);

		cube->render();*/



	}



	earthTheta += float(timer.getDeltaTimeSeconds()) * 6 * speedUpTime / minutesADay;
	change += (float(timer.getDeltaTimeSeconds()) / 2.0f) * 6 * speedUpTime / minutesADay;

	cX = (cosf(glm::radians(change)));
	cY = (cosf(glm::radians(change)));
	cZ = (cosf(glm::radians(change)));
	if (cosf(glm::radians(change)) < 0.6f) cX = 0.6f;
	if (cosf(glm::radians(change)) < 0.6f) cY = 0.6f;
	if (cosf(glm::radians(change)) < 0.6f) cZ = 0.6f;
}