#include "Model.h"
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
void Model::Draw(Shader &shader) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shader);
	}
}

Model::Model(std::string path) {
	//stbi_set_flip_vertically_on_load(true);
	loadModel(path);
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const  aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Error loading scene!" << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('\\'));
	//std::cout << directory << std::endl;
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    	aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    	meshes.push_back(processMesh(mesh,scene));
    }
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vert;
		vert.x = mesh->mVertices[i].x;
		vert.y = mesh->mVertices[i].y;
		vert.z = mesh->mVertices[i].z;
		//std::cout <<"positon"<< vert.x<<" "<<vert.y<<"  "<<vert.z << std::endl;
		vertex.Position = vert;
		vert.x = mesh->mNormals[i].x;
		vert.y = mesh->mNormals[i].y;
		vert.z = mesh->mNormals[i].z;
		//std::cout <<"normal"<< vert.x << " " << vert.y << "  " << vert.z << std::endl;
		vertex.Normal = vert;
		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y= mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		vertices.push_back(vertex);
	}
	for (unsigned int  j = 0; j < mesh->mNumFaces; j++)
	{
		for (unsigned int i = 0; i < mesh->mFaces[i].mNumIndices; i++) {
			indices.push_back(mesh->mFaces[j].mIndices[i]);
		}
	}
	
		aiMaterial* material= scene->mMaterials[mesh->mMaterialIndex];
		// 1. diffuse maps
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "material.diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "material.specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. reflection maps
		std::vector<Texture> reflectionMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "material.ambient");
		textures.insert(textures.end(), reflectionMaps.begin(), reflectionMaps.end());
	
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{	
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size();j++) {
			if (std::strcmp(textures_loaded[i].path.data(), str.C_Str()) == 0) {
				textures.push_back(textures_loaded[j]);
				skip = false;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			texture.id = textureFromFile(str.C_Str(),this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

unsigned int Model::textureFromFile(const char* path,const  std::string& directory) {
	std::string filaName = std::string(path);
	filaName = directory + '/' + filaName;
	
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponets;
unsigned char* data = stbi_load(filaName.c_str(), &width, &height, &nrComponets, 0);
if (data) {
	GLenum format;
	if (nrComponets == 1)
		format = GL_RED;
	else if (nrComponets == 3)
		format = GL_RGB;
	else if (nrComponets == 4)
		format = GL_RGBA;

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	stbi_image_free(data);
}
else {
	std::cout << "texture failed to load at path: " << path << std::endl;
	stbi_image_free(data);
}
	return textureID;
}	




