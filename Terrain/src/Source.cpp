#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"

#include <Model.h>

#include <iostream>
#include <stdbool.h>

#include "Properties.h"
#include "Renderer.h"

// settings

GLFWwindow* init();
void frameStart(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void setVAO(vector <float> vertices);

void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);

// camera
Camera camera(glm::vec3(55,12.5,-33));

float lastX = Properties::SCR_WIDTH / 2.0f;
float lastY = Properties::SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Toggle settings
bool sprint = false; //Move the camera faster

bool Properties::wireframe = false;
bool Properties::useHeightTexture = true;
bool Properties::useFlatShading = false;
bool Properties::fogEnabled = true;
bool Properties::debugPlane = false;
bool Properties::toggleNoiseAlgorithm = true;

int Properties::heightGenerationMethod = 0;

bool updateTerrain = false;

int main() {
	GLFWwindow* window = init();
	if (window == nullptr) return -1;
	
	Renderer renderer;
	FrameBuffer frameBuffer;
	
	renderer.registerLights();
	frameBuffer.setFBOWaterRefraction();
	frameBuffer.setFBOWaterReflection();
	frameBuffer.setFBOWaterDepth();
	frameBuffer.setFBOShadowMap();

	renderer.updateComputeShader(camera);

	glm::vec4 nullClipPlane = glm::vec4(0, -1, 0, 100000);
	
	while (!glfwWindowShouldClose(window)) {
		if (updateTerrain) {
			renderer.updateTerrain(camera);
			updateTerrain = false;
		}
		
		frameStart(window);

		float height = renderer.getWater().getHeight()+1;
		
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.getFBOWaterReflection());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float reflectDist = 2 * (camera.Position.y - height);
		camera.Position.y -= reflectDist;
		camera.Pitch *= -1;
		camera.updateCameraVectors();
		renderer.render(camera, glm::vec4(0, 1, 0, -height+1.0f));

		camera.Position.y += reflectDist;
		camera.Pitch *= -1;
		camera.updateCameraVectors();

		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.getFBOWaterRefraction());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer.render(camera, glm::vec4(0, -1, 0, height));

		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.getFBORefractionDepth());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer.render(camera, glm::vec4(0, -1, 0, height));

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.renderWater(camera, frameBuffer, deltaTime);
		
		renderer.render(camera, nullClipPlane);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	std::cout << "Exiting while loop" << std::endl; //Not hit by crash
	glfwTerminate();
	return 0;
}

#pragma region EXTRA_FUNCS
GLFWwindow* init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(Properties::SCR_WIDTH, Properties::SCR_HEIGHT, "IMAT3907", NULL, NULL);
	if (window == NULL)	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetKeyCallback(window, keyCallBack);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_CLIP_DISTANCE0);
	glCullFace(GL_BACK);

	return window;
}

void frameStart(GLFWwindow* window) {
	glEnable(GL_DEPTH_TEST);
	auto currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	processInput(window);

	glClearColor(Properties::RED, Properties::GREEN, Properties::BLUE, 1.0);
}

void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		std::cout << "Cam pos: " << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z << std::endl;
		std::cout << "Cam view: " << camera.Pitch << ", " << camera.Yaw << std::endl;
	}
	
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) sprint = true;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) sprint = false;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) Properties::wireframe = !Properties::wireframe;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) Properties::useHeightTexture = !Properties::useHeightTexture;
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) Properties::useFlatShading = !Properties::useFlatShading;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) Properties::fogEnabled = !Properties::fogEnabled;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		Properties::toggleNoiseAlgorithm = !Properties::toggleNoiseAlgorithm;
		updateTerrain = true;
	}
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		if (Properties::heightGenerationMethod < 2) {
			Properties::heightGenerationMethod++;
		} else {
			Properties::heightGenerationMethod = 0;
		}
		updateTerrain = true;

		if (Properties::heightGenerationMethod == 0) { std::cout << "TERRAIN GEN METHOD set to GPU Compute" << std::endl; }
		if (Properties::heightGenerationMethod == 1) { std::cout << "TERRAIN GEN METHOD set to GPU Evaluation" << std::endl; }
		if (Properties::heightGenerationMethod == 2) { std::cout << "TERRAIN GEN METHOD set to Height Map Texture" << std::endl; }
	}
	
}
#pragma endregion
#pragma region PROVIDED_CODE
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(CAM_FORWARD, deltaTime, sprint);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(CAM_BACKWARD, deltaTime, sprint);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(CAM_LEFT, deltaTime, sprint);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(CAM_RIGHT, deltaTime, sprint);
	//Can these be in the default project for future years - small thing but feels very natural to use!
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(CAM_UP, deltaTime, sprint);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(CAM_DOWN, deltaTime, sprint);
}



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
#pragma endregion