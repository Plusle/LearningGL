#ifndef _MESH_H_
#define _MESH_H_

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <vector>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_coord;
	glm::vec3 tangents;
	glm::vec3 bitangents;
};

struct Texture {
	GLuint id;
	std::string type;
	std::string path;
};

class Mesh {
	public:
		Mesh(std::vector<Vertex> vertrices, std::vector<GLuint> indices, std::vector<Texture> textures) {
			this->vertrices = vertrices;
			this->indices = indices;
			this->textures = textures;
			initialize();
		}
		void Draw(Shader shader);

	private:
		std::vector<Vertex> vertrices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;

		GLuint vertex_buffer, index_buffer, vertex_array;

		void initialize();

};

void Mesh::initialize() {
	glGenBuffers(1, &vertex_buffer);
	glGenVertexArrays(1, &vertex_array);
	glGenBuffers(1, &index_buffer);

	glBindVertexArray(vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertrices.size(), &vertrices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangents));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangents));
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);
};

void Mesh::Draw(Shader shader) {
	GLuint diff_no = 1;
	GLuint spec_no = 1;
	GLuint norm_no = 1;
	GLuint heig_no = 1;

	for (GLuint iter = 0; iter != textures.size(); ++iter) {
		glActiveTexture(GL_TEXTURE0 + iter);
		std::string type = textures[iter].type;
		std::string no;
		if (type == "texture_diffuse")
			no = std::to_string(diff_no++);
		else if (type == "texture_specular")
			no = std::to_string(spec_no++);
		else if (type == "texture_normal")
			no = std::to_string(norm_no++);
		else if (type == "texture_height")
			no = std::to_string(heig_no++);

		std::string sampler2d = type + no;
		//shader.setInt(sampler2d.c_str(), iter);
		glUniform1i(glGetUniformLocation(shader.ID, sampler2d.c_str()), iter);
		glBindTexture(GL_TEXTURE_2D, textures[iter].id);
	}

	glBindVertexArray(vertex_array);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(0);
}

#endif