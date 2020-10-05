#pragma once
#define GLEW_STATIC
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtc/type_ptr.hpp"
#include"Shader.h"
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<assimp/anim.h>
#include<vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};
struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};
class Mesh {
public:
	std::vector<Vertex> vertics;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	Mesh(float vertics[]);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader &shader);
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh(); 
};
