#include "Includes.h"
#include "VertexData.h"

// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


// Camera settings
//							  width, heigh, near plane, far plane
Camera_settings camera_settings{ 1600, 900, 0.01, 1000.0 };

//Timer
Timer timer;

// Instantiate the camera object with basic data
Camera camera(camera_settings, glm::vec3(0.0f, 3.0f, 0.0f));

double lastX = camera_settings.screenWidth / 2.0f;
double lastY = camera_settings.screenHeight / 2.0f;

//Used in scene to determine how much we are speeding time up by
float speedUpTime = 1.0f;

//Checks for button presses
bool bHDR = false;
bool bGreyScale = false;
bool bHDRKeyPressed = false;
bool bGreyScaleKeyPressed = false;

//Exposure rate of the HDR colour
float exposure = 1.0f;

int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(camera_settings.screenWidth, camera_settings.screenHeight, "Village Cycle", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Set the callback functions
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Rendering settings
	glfwSwapInterval(1);		// glfw enable swap interval to match screen v-sync
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //Enables face culling
	glFrontFace(GL_CCW);//Specifies which winding order if front facing
	glEnable(GL_MULTISAMPLE); //Enables Multisampling

	fstream outFile("LogSeed.txt", ios::out | ios::app);
	if (outFile.is_open())
	{
		outFile << "-------------------------------" << endl;
		outFile.close();
	}
	else
		cout << "Could not open file!" << endl;

	//Creating the scene we render into the hdr color buffer
	DayAndNightScene grasslandsWithDayAndNightCycle(camera_settings);

	//Hdr framebuffer setup with depth and colour attachements
	HDR hdr(camera_settings);


	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);
		timer.tick();

		

		//Binding the HDR framebuffer
		hdr.bindHDRFramebuffer();
		//Render our scene into the frambuffer
		grasslandsWithDayAndNightCycle.render(camera, timer, speedUpTime);
		//Un bind the framebuffer
		hdr.unBindHDRFramebuffer();
		//render the frambuffer content
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		hdr.render(camera, width, height, bHDR, exposure, bGreyScale);

		//Checking if have HDR enabled or not
		if (bHDRKeyPressed)
		{
			string hdr = bHDR ? "On" : "Off";
			cout << "HDR is " << hdr << endl;
		}
		
		//Checking if have grey scale enabled or not
		if (bGreyScaleKeyPressed)
		{
			string greyscale = bGreyScale ? "On" : "Off";
			cout << "Grey scale is " << greyscale << endl;
		}
		

		// glfw: swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	timer.updateDeltaTime();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(FORWARD, timer.getDeltaTimeSeconds());
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, timer.getDeltaTimeSeconds());
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT, timer.getDeltaTimeSeconds());
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, timer.getDeltaTimeSeconds());

	//Used to speed up time it takes for a day cycle to pass
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
		speedUpTime += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		speedUpTime -= 1.0f;

	//Displays greyscale image of the scene
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && !bGreyScaleKeyPressed)
	{
		bGreyScale = !bGreyScale;
		bGreyScaleKeyPressed = true;

	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE)
		bGreyScaleKeyPressed = false;


	//Displays the scene in HDR color space
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !bHDRKeyPressed)
	{
		bHDR = !bHDR;
		bHDRKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		bHDRKeyPressed = false;

	//Changes the exposure rate of the HDR color
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		if (exposure > 0.0f)
			exposure -= 0.001f;
		else
			exposure = 0.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		exposure += 0.001f;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	glViewport(0, 0, width, height);
	camera.updateScreenSize(width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	 if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		camera.processMouseMovement(xoffset, yoffset);
	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScroll(yoffset);
}

