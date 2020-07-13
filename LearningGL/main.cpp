#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "FPSCamera.h"
#include "Model.h"

#include <iostream>

const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;
const GLfloat PI = 3.14;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat lastX = (GLfloat)SCR_WIDTH / 2;
GLfloat lastY = (GLfloat)SCR_HEIGHT / 2;

bool firstTimeFocus = true;

FPSCamera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void process_input(GLFWwindow* window);

int main(int argc, char** argv) {

	// glfw initializing
	//---------------------------------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window
	//---------------------------------------------------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearningOpenGLCN", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to initialize window." << std::endl;
		glfwTerminate();
		return -1;
	}

	// set up current window
	glfwMakeContextCurrent(window);
	// bind callback function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);	// be ware that this and next callback function's parameters must be double-type
	glfwSetScrollCallback(window, scroll_callback);

	// no cursor window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// load opengl function address
	//---------------------------------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to load OpenGL function" << std::endl;
		glfwTerminate();
		return -1;
	}

	Shader shader = Shader("model.vert", "model.frag");

	Model nano = Model("nano/nanosuit.obj");

	glEnable(GL_DEPTH_TEST);

	// main loop
	//---------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {
		process_input(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(camera.getFOV(), (float)(SCR_WIDTH / SCR_HEIGHT), 0.01f, 100.0f);
		glm::mat4 view = camera.getView();

		shader.use();
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		nano.Draw(shader);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	// waste disposal
	//---------------------------------------------------------------


	glfwTerminate();
	return 0;
}


// framebuffer size callback function
//---------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* w, int width, int height) {
	glViewport(0, 0, width, height);
}
// mouse callback function
//---------------------------------------------------------------
void mouse_callback(GLFWwindow* w, double xpos, double ypos) {
	//std::cout << "x,y: " << xpos << " " << ypos << std::endl;

	if (firstTimeFocus) {
		lastX = (GLfloat)xpos;
		lastY = (GLfloat)ypos;
		firstTimeFocus = false;
	}

	// offset = new pos - old pos
	GLfloat xoffset = (float)(xpos - lastX);
	GLfloat yoffset = -(float)(ypos - lastY);	// notice that the origin of screen is on the left top, so if move up, ypos decreases.
	lastX = (GLfloat)xpos;
	lastY = (GLfloat)ypos;
	
	camera.rotate(xoffset, yoffset);
}

// scroll callback function
//---------------------------------------------------------------
void scroll_callback(GLFWwindow* w, double xoffset, double yoffset) {
	camera.scroll(yoffset);
}

// hit escape to close window
//---------------------------------------------------------------
void process_input(GLFWwindow* w) {
	if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(w, true);

	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS)
		camera.walk(CAMERA_MOVE_FORWARD, deltaTime);
	if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS)
		camera.walk(CAMERA_MOVE_BACKWARD, deltaTime);
	if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS)
		camera.walk(CAMERA_MOVE_LEFT, deltaTime);
	if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS)
		camera.walk(CAMERA_MOVE_RIGHT, deltaTime);

}