#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "Timer.h"
#include "Model.h"
#include "Circle.h"
#include "Camera.h"
#include "Sphere.h"
#include "TexturedQuad.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "PrincipleAxes.h"
#include "Cube.h"
#include "Shader.h"
class DayAndNightScene
{
private:
	// Instantiate the camera object with basic data

	//Setup shaders to use inside the scene
	Shader earth = { "Resources\\Shaders\\PhongTexture_shader.vs", "Resources\\Shaders\\PhongTexture_shader.fs" };
	Shader sunAndMoon = { "Resources\\Shaders\\OnlyColour_shader.vs", "Resources\\Shaders\\OnlyColour_shader.fs" };

	//Models loaded into the scene
	Model grass = { "Resources\\Models\\grassLowPoly.fbx", 20000, 200, 200, 1575507318 };
	Model bush = { "Resources\\Models\\bush.fbx", 1000, 323, 323, 1575507318 };
	Model rock = { "Resources\\Models\\rock.fbx", 500, 300, 300, 1575507318 };
	Model house = { "Resources\\Models\\House\\house.fbx", 15, 180, 180 ,1575507318 };
	Model house2 = { "Resources\\Models\\House\\house2.fbx", 30, 110, 150 ,1575507318 };

	//Camera settings for resolution setting
	Camera_settings settings;

	//Storing textures
	GLuint demoTexture;
	GLuint grassTexture; 
	GLuint grassTexture2; 

	//Primitive objects
	Sphere *earthModel = nullptr;
	Cube *cube = nullptr;

	//Used for rotation of the light in the scene
	float earthTheta = 0.0f;

	//Color of the background
	float cX = 83.0f;
	float cY = 131.0f;
	float cZ = 171.0f;

	//Used to change the color of the background
	float change = 0;

	//Specifies the time it takes for the day cycle to complete
	float minutesADay = 10.0f;
public:
	//Public contructor, takes in camera settings for sceen width and screen height
	DayAndNightScene(Camera_settings);

	//Decontructor
	~DayAndNightScene();

	//Function used to render the scene
	void render(Camera, Timer, float);

	void renderLights(Camera, Timer, float);
};