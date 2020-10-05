#include "Texture.h"
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include"stb_image.h"
#include<iostream>
Texture::Texture(const char* name,int num) {
	unsigned int textureB;
	glGenTextures(1, &textureB);
	glActiveTexture(GL_TEXTURE1+num-1);
	glBindTexture(GL_TEXTURE_2D, textureB);
	 int width, height, nrChannel;
	unsigned char* data2 = stbi_load(name, &width, &height, &nrChannel, 0);
	if (data2) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "load image error!!" << std::endl;
	}
	stbi_image_free(data2);
	text = textureB;
}
