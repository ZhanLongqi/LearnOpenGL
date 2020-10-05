#pragma once
#include<String>
class Shader
{
public:
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

	std::string vertexString;
	std::string fragmentString;
	const char* vertexSource;
	const char* fragmentSource;
	unsigned int ID;
	void use();
	void checkCompileErrors(unsigned int shader, std::string type);
};

