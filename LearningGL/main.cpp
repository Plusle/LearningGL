#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "Shader.h"
#include "FPSCamera.h"


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

void RenderOther(GLuint, GLuint, Shader);
void RenderModel(GLuint, GLuint, Shader);
void RenderOutline(GLuint, Shader);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void process_input(GLFWwindow* window);
GLuint load_texture(char const* path);

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
	if (!gladLoadGL()) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// vertrices
	float cubeVertices[] = {
		// positions          // texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};

	GLuint cube_buffer, cube_array;
	glGenBuffers(1, &cube_buffer);
	glGenVertexArrays(1, &cube_array);
	glBindVertexArray(cube_array);
	glBindBuffer(GL_ARRAY_BUFFER, cube_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));

	GLuint floor_buffer, floor_array;
	glGenBuffers(1, &floor_buffer);
	glGenVertexArrays(1, &floor_array);
	glBindVertexArray(floor_array);
	glBindBuffer(GL_ARRAY_BUFFER, floor_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));

	GLuint cube_texture = load_texture("marble.jpg");
	GLuint floor_texture = load_texture("floor.png");

	Shader shader("scene.vert", "scene.frag");
	shader.use();
	shader.setInt("texture0", 0);
	Shader shaderOutline("scene.vert", "outline.frag");


	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_NOTEQUAL, 1, 0xff);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//glStencilMask(0xff);

	// main loop
	//---------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		process_input(window);
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		// Render floor
		RenderOther(floor_array, floor_texture, shader);

		// Render Cube		
		// 1. Render Model

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		RenderModel(cube_array, cube_texture, shader);

		//glStencilFunc(GL_ALWAYS, 1, 0xff);
		//glStencilMask(0xff);
		//RenderModel(cube_array, cube_texture, shader);
		//glStencilFunc(GL_NOTEQUAL, 1, 0xff);

		// 2. Render Outline
		
		glStencilMask(0x00);
		glStencilFunc(GL_NOTEQUAL, 1, 0xff);
		glDisable(GL_DEPTH_TEST);
		RenderOutline(cube_array, shaderOutline);
		glEnable(GL_DEPTH_TEST);
		glStencilMask(0xff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glDisable(GL_STENCIL_TEST);





		//glStencilFunc(GL_NOTEQUAL, 1, 0xff);
		//glStencilMask(0x00);
		//glDisable(GL_DEPTH_TEST);
		//RenderOutline(cube_array, shaderOutline);
		//glEnable(GL_DEPTH_TEST);
		//glStencilMask(0xff);



		
		glfwSwapBuffers(window);
	}
	
	// waste disposal
	//---------------------------------------------------------------


	glfwTerminate();
	return 0;
}

void RenderOther(GLuint vao, GLuint texture, Shader shader) {
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = camera.getView();
	glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), (static_cast<float>(SCR_WIDTH) / SCR_HEIGHT), 0.1f, 200.0f);

	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader.use();
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.use();
	shader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void RenderModel(GLuint vao, GLuint texture, Shader shader) {
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = camera.getView();
	glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), (static_cast<float>(SCR_WIDTH) / SCR_HEIGHT), 0.1f, 200.0f);

	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader.use();
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	shader.setMat4("model", glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0f)));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	shader.setMat4("model", glm::translate(model, glm::vec3(-1.0f, 0.0f, 1.5f)));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void RenderOutline(GLuint vao, Shader shader) {
	glm::mat4 view = camera.getView();
	glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), (static_cast<float>(SCR_WIDTH) / SCR_HEIGHT), 0.1f, 200.0f);
	
	float scale = 1.05f;
	glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
	glm::mat4 translate1 = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 1.0f));
	glm::mat4 translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 1.5f));

	glm::mat4 cube1 = translate1 * scale_matrix;
	glm::mat4 cube2 = translate2 * scale_matrix;
	glBindVertexArray(vao);

	shader.use();
	shader.setMat4("view", view);
	shader.setMat4("projection", projection); 
	shader.setMat4("model", cube1);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	shader.setMat4("model", cube2);
	glDrawArrays(GL_TRIANGLES, 0, 36);
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

// function for loading texture temporarily 
//---------------------------------------------------------------
GLuint load_texture(char const* path)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	GLint width, height, nrComponents;
	GLubyte* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}