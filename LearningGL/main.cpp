#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "FPSCamera.h"

#include <iostream>

const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;

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

	// a box with face
	//---------------------------------------------------------------
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};


	// vertex buffer and attirbute array
	//---------------------------------------------------------------
	GLuint entity_array, vertex_buffer;
	//GLuint EBO;
	glGenBuffers(1, &vertex_buffer);
	glGenVertexArrays(1, &entity_array);
	//glGenBuffers(1, &EBO);
	glBindVertexArray(entity_array);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// first attribute is vertex postion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);

	GLuint light_array;
	glGenVertexArrays(1, &light_array);
	glBindVertexArray(light_array);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);
	glEnableVertexAttribArray(0);

	// shader program initializing
	//---------------------------------------------------------------
	Shader entity_shader("entity.vert", "entity.frag");
	Shader light_shader("light.vert", "light.frag");
	// when only 1 sampler, some of graphics card can do this implicitly

	entity_shader.use();
	entity_shader.setVec3("entity_color", glm::vec3(1.0f, 0.5f, 0.31f));
	entity_shader.setVec3("light_color", glm::vec3(1.0f));


	
	
	// main loop
	//---------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {
		process_input(window);

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(entity_array);

		entity_shader.use();
		entity_shader.setVec3("entity_color", glm::vec3(1.0f, 0.5f, 0.31f));
		entity_shader.setVec3("light_color", glm::vec3(1.0f));
		entity_shader.setVec3("light_position", glm::vec3(1.2f, 1.0f, 2.0f));
		entity_shader.setVec3("view_position", camera.getPosition());

		glm::mat4 model_entity = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);
		entity_shader.setMat4("model", model_entity);
		entity_shader.setMat4("view", camera.getView());
		entity_shader.setMat4("projection", projection);
		entity_shader.setVec3("light_color", glm::vec3(1.0f));
		entity_shader.setVec3("entity_color", glm::vec3(1.0f, 0.5f, 0.31f));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glBindVertexArray(light_array);
		light_shader.use();
		glm::mat4 model_light = glm::translate(glm::mat4(1.0f), glm::vec3(1.2f, 1.0f, 2.0f));
		model_light = glm::scale(model_light, glm::vec3(0.2f));
		light_shader.setMat4("model", model_light);
		light_shader.setMat4("view", camera.getView());
		light_shader.setMat4("projection", projection);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// waste disposal
	//---------------------------------------------------------------
	//myProgram.kill();
	
	glDeleteVertexArrays(1, &entity_array);
	glDeleteVertexArrays(1, &light_array);
	glDeleteBuffers(1, &vertex_buffer);

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
	std::cout << "x,y: " << xpos << " " << ypos << std::endl;

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

	//GLfloat currentFrame = (GLfloat)glfwGetTime();
	//deltaTime = currentFrame - lastFrame;
	//lastFrame = currentFrame;
	//GLfloat cameraSpeed = 3.0f * deltaTime;
	//if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS)
	//	cameraPos += cameraSpeed * glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
	//if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS)
	//	cameraPos -= cameraSpeed * glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
	//if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS)
	//	cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
	//if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS)
	//	cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));

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