#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class Light {
	public:
		Light() = default;
		Light(glm::vec3 a, glm::vec3 d, glm::vec3 s, GLfloat c, GLfloat l, GLfloat q) : 
			  amibent(a), diffuse(d), specular(s), constant(c), linear(l), quadratic(q) {}

		virtual void write_basic_info();

	protected:
		glm::vec3 amibent;
		glm::vec3 diffuse;
		glm::vec3 specular;

		GLfloat constant;
		GLfloat linear;
		GLfloat quadratic;
};

class ParallelLight : public Light {
	public:
		ParallelLight() = default;
		
	private:
		glm::vec3 direction;
};

class PointLight : public Light {
	public:

	private:
		glm::vec3 position;
};

class Spotlight : public Light {
	public:

	private:
		glm::vec3 direction;
		glm::vec3 position;
		GLfloat inner_cutoff;
		GLfloat outter_cutoff;
};




#endif _LIGHT_H_