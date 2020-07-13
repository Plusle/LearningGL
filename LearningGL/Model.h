#ifndef _MODEL_H_
#define _MODEL_H_


#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

GLuint TextureFromFile(const char* path, const std::string& directory);

class Model {
	public:
		Model(const char* path) {
			LoadModel(path);
		}
		void Draw(Shader shader);

	private:
		std::vector<Mesh> meshes;
		std::string directory;

		// record every texture which have been loaded and reuse from it;
		std::vector<Texture> loaded;

		void LoadModel(std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadMaterialTexture(aiMaterial* material, aiTextureType type, std::string typeName);
};

void Model::Draw(Shader shader) {
	for (GLuint iter = 0; iter != meshes.size(); ++iter) 
		meshes[iter].Draw(shader);
}

void Model::LoadModel(std::string path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR: Assimp::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
	for (GLuint iter = 0; iter < node->mNumMeshes; iter++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[iter]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (GLuint iter = 0; iter < node->mNumChildren; iter++)
		ProcessNode(node->mChildren[iter], scene);
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
	// new idea about coding style: _name means temporary var
	std::vector<Vertex> _vertrices;
	std::vector<GLuint> _indices;
	std::vector<Texture> _textures;

	for (GLuint iter = 0; iter < mesh->mNumVertices; ++iter) {
		Vertex _vertex;
		// position
		glm::vec3 _vertex_position;
		_vertex_position.x = mesh->mVertices[iter].x;
		_vertex_position.y = mesh->mVertices[iter].y;
		_vertex_position.z = mesh->mVertices[iter].z;
		_vertex.position = _vertex_position;

		// normal
		glm::vec3 _vertex_normal;
		_vertex_normal.x = mesh->mNormals[iter].x;
		_vertex_normal.y = mesh->mNormals[iter].y;
		_vertex_normal.z = mesh->mNormals[iter].z;
		_vertex.normal = _vertex_normal;

		// texture coordinate
		glm::vec2 _tex_coord;
		if (mesh->mTextureCoords[0]) {
			_tex_coord.x = mesh->mTextureCoords[0][iter].x;
			_tex_coord.y = mesh->mTextureCoords[0][iter].y;
			
		} else {
			_tex_coord = glm::vec2(0.0f);
		}
		_vertex.tex_coord = _tex_coord;

		// tangents
		glm::vec3 _tangents;
		_tangents.x = mesh->mTangents[iter].x;
		_tangents.y = mesh->mTangents[iter].y;
		_tangents.z = mesh->mTangents[iter].z;
		_vertex.tangents = _tangents;

		// bitangents
		glm::vec3 _bitangents;
		_bitangents.x = mesh->mBitangents[iter].x;
		_bitangents.y = mesh->mBitangents[iter].y;
		_bitangents.z = mesh->mBitangents[iter].z;
		_vertex.bitangents = _bitangents;

		_vertrices.push_back(_vertex);
	}

	// fix indices
	for (GLuint iter_face = 0; iter_face < mesh->mNumFaces; ++iter_face) {
		aiFace face = mesh->mFaces[iter_face];
		for (GLuint iter_index = 0; iter_index < face.mNumIndices; ++iter_index)
			_indices.push_back(face.mIndices[iter_index]);
	}

	// fix textures
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuse_map = LoadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
		_textures.insert(_textures.end(), diffuse_map.begin(), diffuse_map.end());
		std::vector<Texture> specular_map = LoadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
		_textures.insert(_textures.end(), specular_map.begin(), specular_map.end());
		std::vector<Texture> normal_map = LoadMaterialTexture(material, aiTextureType_NORMALS, "texture_normal");
		_textures.insert(_textures.end(), normal_map.begin(), normal_map.end());
		std::vector<Texture> height_map = LoadMaterialTexture(material, aiTextureType_HEIGHT, "texture_height");
		_textures.insert(_textures.end(), height_map.begin(), height_map.end());
	}

	return Mesh(_vertrices, _indices, _textures);
}

std::vector<Texture> Model::LoadMaterialTexture(aiMaterial* material, aiTextureType type, std::string type_name) {
	std::vector<Texture> _textures_set;
	for (GLuint iter = 0; iter < material->GetTextureCount(type); ++iter) {
		aiString str;
		material->GetTexture(type, iter, &str);
		GLboolean is_loaded = false;
		for (GLuint i = 0; i < loaded.size(); ++i) {
			if (std::strcmp(loaded[i].path.c_str(), str.C_Str()) == 0) {
				_textures_set.push_back(loaded[i]);
				is_loaded = true;
				break;
			}
		}

		if (!is_loaded) {
			Texture _texture;
			_texture.id = TextureFromFile(str.C_Str(), this->directory);
			_texture.type = type_name;
			_texture.path = str.C_Str();
			_textures_set.push_back(_texture);
			loaded.push_back(_texture);
		}
	}
	return _textures_set;
}

GLuint TextureFromFile(const char* path, const std::string& directory) {
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	
	GLuint _id;
	glGenTextures(1, &_id);

	GLint width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, _id); 
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

		stbi_image_free(data);
	} else {
		std::cout << "Failed to load texture file:" << filename << std::endl;
		stbi_image_free(data);
	}

	return _id;
}

#endif _MODEL_H_