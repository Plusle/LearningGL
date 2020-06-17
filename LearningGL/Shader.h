#pragma once
#ifndef _SHADER_H_
#define _SHADER_H_

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

class Shader {
	public:
		GLuint ID;

		Shader(const char* vertexPath, const char* fragmentPath) {
			// self-evident variables
			GLuint vShader, fShader;
			std::string vertexCode, fragmentCode; 
			std::ifstream vShaderFile, fShaderFile;	
			std::stringstream vShaderStream, fShaderStream;

			// exception setting
			vShaderFile.exceptions(std::fstream::failbit | std::fstream::badbit);
			fShaderFile.exceptions(std::fstream::failbit | std::fstream::badbit);

			// try to transfer file data into sstream
			try {
				vShaderFile.open(vertexPath);
				fShaderFile.open(fragmentPath);

				vShaderStream << vShaderFile.rdbuf();
				fShaderStream << fShaderFile.rdbuf();

				vShaderFile.close();
				fShaderFile.close();
			} catch (std::ifstream::failure e){
				std::cout << "Failed to read shader code." << std::endl;
			}

			// c-stylize
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

			const char* vShaderCode = vertexCode.c_str();
			const char* fShaderCode = fragmentCode.c_str();

			// vertex shader
			vShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vShader, 1, &vShaderCode, NULL);
			glCompileShader(vShader);
			CheckShaderStatus(vShader, "vertex shader");

			// fragment shader
			fShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fShader, 1, &fShaderCode, NULL);
			glCompileShader(fShader);
			CheckShaderStatus(fShader, "fragment shader");

			// link program
			ID = glCreateProgram();
			glAttachShader(ID, vShader);
			glAttachShader(ID, fShader);
			glLinkProgram(ID);
			CheckShaderStatus(ID, "PROGRAM");

			// delete useless waste
			glDeleteShader(vShader);
			glDeleteShader(fShader);
		}

		void use() {
			glUseProgram(ID);
		}

		void kill() {
			glDeleteProgram(ID);
		}

		void setInt(const char* u, int v) {
			glUniform1i(glGetUniformLocation(ID, u), v);
		}

		void setFloat(const char* u, float v) {
			glUniform1f(glGetUniformLocation(ID, u), v);
		}

		void setBool(const char* u, bool v) {
			glUniform1i(glGetUniformLocation(ID, u), (int)v);
		}

		void setVec2(const char* u, float v1, float v2) {
			glUniform2f(glGetUniformLocation(ID, u), v1, v2);
		}

		void setVec2(const char* u, const glm::vec2 &v) const {
			glUniform2fv(glGetUniformLocation(ID, u), 1, &v[0]);
		}

		void setVec3(const char* u, float v1, float v2, float v3) {
			glUniform3f(glGetUniformLocation(ID, u), v1, v2, v3);
		}

		void setVec3(const char* u, const glm::vec3 &v) const {
			glUniform3fv(glGetUniformLocation(ID, u), 1, &v[0]);
		}

		void setVec4(const char* u, float v1, float v2, float v3, float v4) {
			glUniform4f(glGetUniformLocation(ID, u), v1, v2, v3, v4);
		}

		void setVec4(const char* u, const glm::vec4 &v) const {
			glUniform4fv(glGetUniformLocation(ID, u), 1, &v[0]);
		}

		void setMat2(const char* u, const glm::mat2 &m) const {
			glUniformMatrix2fv(glGetUniformLocation(ID, u), 1, GL_FALSE, &m[0][0]);
		}

		void setMat3(const char* u, const glm::mat3 &m) const {
			glUniformMatrix3fv(glGetUniformLocation(ID, u), 1, GL_FALSE, &m[0][0]);
		}

		void setMat4(const char* u, const glm::mat4 &m) const {
			glUniformMatrix4fv(glGetUniformLocation(ID, u), 1, GL_FALSE, &m[0][0]);
		}


	private:

		void CheckShaderStatus(GLuint shader, const char* type) {
			GLint success;
			GLchar infolog[512];

			if (type != "PROGRAM") {
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (!success) {
					glGetShaderInfoLog(shader, 512, NULL, infolog);
					std::cout << "Faild to compile " << type << " due to:\n" << infolog << std::endl;
				}
			} else {
				glGetProgramiv(shader, GL_LINK_STATUS, &success);
				if (!success) {
					glGetProgramInfoLog(shader, 512, NULL, infolog);
					std::cout << "Failed to link program due to:\n" << infolog << std::endl;
				}
			}
		}
};

#endif