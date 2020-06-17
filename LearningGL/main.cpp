#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
		//  x      y      z      texture
		-0.5f, -0.5f, 0.0f,		 0.0f,  0.0f,
		-0.5f,  0.5f, 0.0f,		 1.0f,  0.0f,
 		 0.5f,  0.5f, 0.0f,		 1.0f,  1.0f,
 	 	 0.5f, -0.5f, 0.0f,		 0.0f,  1.0f
	};

	// draw indices
	// lb -> lt -> rt -> rb
	GLuint indices[] = {
		0, 1, 2, 
		2, 3, 0
	};

	// vertex buffer and attirbute array
	//---------------------------------------------------------------
	GLuint VAO, VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	// bind VAO firstly ensure everything in control
	// but the fact is only EBO binding behaivor and vertex attribute pointer saved
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// first attribute is vertex postion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);

	// create textures
	//---------------------------------------------------------------
	GLuint tex_Background;
	
	// the texture var is established ralationship with parameters and texture data
	// generate first texture: the background 'container', it's a rgb pic with file format of jpg, NO transparency
	glGenTextures(1, &tex_Background);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_Background);

	// warp setting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	// mipmap setting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// flip the image because the origin in left bottom but image's in left top
	stbi_set_flip_vertically_on_load(true);

	// convert image into byte array
	GLint width, height, nrChannels;
	unsigned char* data1 = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data1) {
		//target: 2D, mipmap level: 0 for set manually, pic format, width, height, legacy burden 0 eternally, format, datatype, data
		// note that the first GL_RGB is int while the latter is GLenum (for what?)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
		
		// set mipmap automatically otherwise do it manually
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load background." << std::endl;
	}

	// good practice free the memory immediately
	stbi_image_free(data1);

	// shader program initializing
	//---------------------------------------------------------------
	Shader myProgram("vertex.gls", "fragment.gls");

	// when only 1 sampler, some of graphics card can do this implicitly
	//myProgram.use();
	//myProgram.setInt("background", 0);			

	
	
	// main loop
	//---------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {
		process_input(window);

		glClearColor(0.0f, 0.1f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//some graphics cards' feature can make us omit the following statment.
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_Background);

		glBindVertexArray(VAO);
		myProgram.use();

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.0f));
		trans = glm::rotate(trans, glm::radians(20.0f), glm::vec3(0.0, 0.0, 1.0));
		myProgram.setMat4("transform", trans);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	// waste disposal
	//---------------------------------------------------------------
	myProgram.kill();
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(1, &tex_Background);

	glfwTerminate();
	return 0;
}