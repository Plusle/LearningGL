#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

#include <iostream>

// framebuffer size callback function
//---------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* w, int width, int height) {
	glViewport(0, 0, width, height);
}
// hit escape to close window
//---------------------------------------------------------------
void process_input(GLFWwindow* w) {
	if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(w, true);

	/*
	static GLfloat move = 0.0f;
	if (glfwGetKey(w, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		if (move <= 0.0f)
			move += 0.01f;
	}

	if (glfwGetKey(w, GLFW_KEY_LEFT) == GLFW_PRESS) {
		if (move >= 0.0f)
			move -= 0.01f;
	}
	
	wait to change uniform to move the primitive, 
	need to figure out the way to control static var, and how to balance the shader program calling and changing uniform var
	*/
}

int main(int argc, char** argv) {

	// glfw initializing
	//---------------------------------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window
	//---------------------------------------------------------------
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearningOpenGLCN", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to initialize window." << std::endl;
		glfwTerminate();
		return -1;
	}

	// set up current window
	glfwMakeContextCurrent(window);
	// bind framesize callback function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// load opengl function address
	//---------------------------------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to load OpenGL function" << std::endl;
		glfwTerminate();
		return -1;
	}

	// a triangle with interpolated color
	//---------------------------------------------------------------
	GLfloat vertices[] = {
	//  x      y      z         r     g     b     a
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
		 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 0.0f
	};

	// vertex buffer and attirbute array
	//---------------------------------------------------------------
	GLuint VAO, VBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// first attribute is vertex postion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)0);
	glEnableVertexAttribArray(0);
	// the second is rgba-value
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);

	// shader program initializing
	//---------------------------------------------------------------
	Shader myProgram("vertex.gls", "fragment.gls");

	// main loop
	//---------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {
		process_input(window);

		glClearColor(0.3f, 0.2f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		myProgram.use();
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	// waste disposal
	//---------------------------------------------------------------
	myProgram.kill();
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}