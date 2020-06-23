#pragma once
#ifndef _FPSCAMERA_H_
#define _FPSCAMERA_H_

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Default camera values
const float YAW = 0.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

enum movement {
	CAMERA_MOVE_FORWARD,
	CAMERA_MOVE_BACKWARD,
	CAMERA_MOVE_LEFT,
	CAMERA_MOVE_RIGHT
};

class FPSCamera {
	public:
		// all by default setting
		FPSCamera(glm::vec3 position) : position(position) {
			flush();
		}

		// specify everything
		FPSCamera(glm::vec3 position, glm::vec3 direction, glm::vec3 up, GLfloat pitch, GLfloat yaw) {
			this->position = position;
			this->direction = direction;
			this->up = up;
			this->pitch = pitch;
			this->yaw = yaw;
			flush();
		}

		// calculate pitch and yaw before jump into this process
		void rotate(GLfloat xoffset, GLfloat yoffset) {
			xoffset *= SENSITIVITY;
			yoffset *= SENSITIVITY;
			
			this->pitch += yoffset;
			this->yaw += xoffset;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			flush();
		}

		// keep everything in this class as much as possible, so i let the class record lastFrame
		// but after i saw the demonstration, i killed them all.
		void walk(movement m, GLdouble deltaTime) {
			GLfloat cameraSpeed = SPEED * deltaTime;
			if (m == CAMERA_MOVE_FORWARD)
				this->position += cameraSpeed * glm::normalize(glm::vec3(this->direction.x, 0.0f, this->direction.z));
			if (m == CAMERA_MOVE_BACKWARD)
				this->position -= cameraSpeed * glm::normalize(glm::vec3(this->direction.x, 0.0f, this->direction.z));
			if (m == CAMERA_MOVE_LEFT)
				this->position -= cameraSpeed * glm::normalize(glm::cross(this->direction, this->up));
			if (m == CAMERA_MOVE_RIGHT)
				this->position += cameraSpeed * glm::normalize(glm::cross(this->direction, this->up));
			flush();
		}

		void scroll(double yoffset) {
			FOV -= (float)yoffset;
			if (FOV > 100.0f)
				FOV = 100.0f;
			if (FOV < 10.0f)
				FOV = 10.0f;
			flush();
		}

		inline GLfloat getFOV() {
			return this->FOV;
		}

		inline glm::mat4 getView() {
			return this->view;
		}

		// setter here
		

	private:
		glm::mat4 view = glm::lookAt(position, direction, up);

		glm::vec3 position;
		glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);		
		glm::vec3 right;
		
		GLfloat FOV = ZOOM;
		GLdouble yaw = YAW;
		GLdouble pitch = PITCH;

		void flush() {
			glm::vec3 tmp_direct;
			tmp_direct.x =  cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
			tmp_direct.y =  sin(glm::radians(this->pitch));
			tmp_direct.z = -cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
			this->direction = tmp_direct;
			this->view = glm::lookAt(this->position, (this->position) + (this->direction), this->up);
			this->right = glm::cross(this->direction, this->up);
		}

};




#endif