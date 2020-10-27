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
#include <vector>
#include <algorithm>

const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;
const GLfloat PI = 3.14;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat lastX = (GLfloat)SCR_WIDTH / 2;
GLfloat lastY = (GLfloat)SCR_HEIGHT / 2;

bool firstTimeFocus = true;

FPSCamera camera(glm::vec3(0.0f, 0.0f, 3.0f));

typedef struct {
	GLuint vbo;
	GLuint vao;
	GLuint texture;
} VertexObject;

typedef struct {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
} Transform;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void process_input(GLFWwindow* window);
GLuint load_texture(char const* path, bool discard);
void Render(VertexObject vo, Shader shader, Transform tsfm, int num_of_vertices);
void RenderOutline(GLuint vao, Shader shader);	// deprecated
VertexObject get_buffer_array_texture(float* data, int length, const char* tex, bool discard);

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

	float grassVertices[] = {
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};

	VertexObject cube = get_buffer_array_texture(cubeVertices, sizeof(cubeVertices) / sizeof(float), "marble.jpg", false);
	VertexObject floor = get_buffer_array_texture(planeVertices, sizeof(planeVertices) / sizeof(float), "floor.png", false);
	VertexObject grass = get_buffer_array_texture(grassVertices, sizeof(grassVertices) / sizeof(float), "grass.png", true);
	VertexObject glass = get_buffer_array_texture(grassVertices, sizeof(grassVertices) / sizeof(float), "transparent-glass.png", false);

	Shader shader("scene.vert", "scene.frag");
	
	std::vector<glm::vec3> vegetation = {
		glm::vec3(-1.5f, 0.0f, -0.48f),
		glm::vec3(1.5f, 0.0f, 0.51f),
		glm::vec3(0.0f, 0.0f, 0.7f),
		glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f)
	};
	// std::sort(vegetation.begin(), vegetation.end(), [](const glm::vec3& a, const glm::vec3& b) { return a.z < b.z; });

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// main loop
	//---------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {
		process_input(window);
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		Transform tsfm = {
			glm::mat4(1.0f),
			camera.getView(),
			glm::perspective(glm::radians(camera.getFOV()), (static_cast<float>(SCR_WIDTH) / SCR_HEIGHT), 0.1f, 200.0f)
		};

		if (glm::dot(camera.getDirection(), glm::vec3(0.0f, 0.0f, -1.0f)) > 0) {
			std::sort(vegetation.begin(), vegetation.end(), [](const glm::vec3& a, const glm::vec3& b) { return a.z < b.z; });
		} else {
			std::sort(vegetation.begin(), vegetation.end(), [](const glm::vec3& a, const glm::vec3& b) { return a.z > b.z; });
		}

		// Render floor
		Render(floor, shader, tsfm, 6);

		// Render cube
		tsfm.model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f));
		Render(cube, shader, tsfm, 36);
		tsfm.model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
		Render(cube, shader, tsfm, 36);

		// Render grass
		for (const glm::vec3& position : vegetation) {
			tsfm.model = glm::translate(glm::mat4(1.0f), position);
			Render(grass, shader, tsfm, 6);
		}

		// Render glass
		for (const glm::vec3& position : vegetation) {
			tsfm.model = glm::translate(glm::mat4(1.0f), position);
			Render(glass, shader, tsfm, 6);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

VertexObject get_buffer_array_texture(float* data, int length, const char* tex, bool discard) {
	GLuint vbo, vao, texture;
	texture = load_texture(tex, discard);
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * length, data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));

	return { vbo, vao, texture };
}

void Render(VertexObject vo, Shader shader, Transform tsfm, int num_of_vertices) {
	glBindVertexArray(vo.vao);
	glBindTexture(GL_TEXTURE_2D, vo.texture);
	shader.use();
	shader.setMat4("model", tsfm.model);
	shader.setMat4("view", tsfm.view);
	shader.setMat4("projection", tsfm.projection);
	glDrawArrays(GL_TRIANGLES, 0, num_of_vertices);
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
GLuint load_texture(char const* path, bool discard) {
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

		if (discard) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		} else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

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