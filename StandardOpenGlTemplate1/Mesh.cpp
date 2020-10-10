#include "Mesh.h"

Mesh::Mesh(float vertics[])
{
	this->vertics.resize(36);
	memcpy(&this->vertics[0], vertics, 36*8*sizeof(float));
	setupMesh();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertics = vertices;
	this->indices = indices;
	this->textures = textures;
	setupMesh();
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNum = 1;
	unsigned int specularNum = 1;
	unsigned int reflectNum = 1;
	unsigned inti;

	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
			  // retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].type;
		if (name == "material.diffuse")
			number = std::to_string(diffuseNum++);
		else if (name == "material.specular")
			number = std::to_string(specularNum++); // transfer unsigned int to stream
		else if (name == "material.ambient")
			number = std::to_string(reflectNum++);

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader.ID, (name+number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertics.size(), &vertics[0], GL_STATIC_DRAW);

 	glGenBuffers(1, &EBO);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,Normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,TexCoords));

	glBindVertexArray(0);
}
