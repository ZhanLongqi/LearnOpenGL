#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include"Shader.h"
#include<string>
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtc/type_ptr.hpp"
#include"Camera.h"
#include"Model.h"
#include"stb_image.h"
#include<map>
#include<vector>
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
unsigned int loadCubemap(std::vector<std::string> faces);
bool firstMouse = true;
glm::vec3 cameraPos(0.f, 0.f, 5.f);
float yaw = -180;
float pitch = 0;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = 800.f / 2.f;
float lastY = 800.f / 2.f;
float fov = 45.f;
bool lightMove = false;
glm::vec3 lightPos(0.f, 0.0f, 0.0f);
Camera camera(cameraPos, pitch, yaw, glm::vec3(0.f, 1.f, 0.f));
void loadTexture(const char* fileName, unsigned int bound);
void showMat(glm::mat4);
int main(int argc,char* argv[]) {
	std::string path = argv[0];
	glfwInit();
	glfwInitHint(GLFW_VERSION_MAJOR, 3);
	glfwInitHint(GLFW_VERSION_MINOR, 3);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpengGl", NULL, NULL);
	//stbi_set_flip_vertically_on_load(true);
	if (window == NULL) {
		std::cout << "failed to init glfw!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glViewport(0, 0, 800, 600);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);
	glfwSetCursorPosCallback(window, mouse_callback);
	if (glewInit() != GLEW_OK) {
		std::cout << "fail to init glew !" << std::endl;
		return -1;
	}
	Shader shader("vertexShader.txt", "fragmentShader.txt");
	Shader lightShader("vertexShader.txt", "LightShader.txt");
	Shader renderShader("renderVertexShader.txt", "renderFragmentShader.txt");
	Shader skyboxShader("skybox_vertexShader.txt", "skybox_fragmentShader.txt");
	//Model model("C:\\work\\cpp\\OpenglExerciseProj02\\StandardOpenGlTemplate1\\nanosuit\\nanosuit.obj");
	Model lightModel("C:\\work\\cpp\\OpenglExerciseProj02\\StandardOpenGlTemplate1\\cube\\cube.obj");


	loadTexture("blending_transparent_window.png", 1);
	loadTexture("bobobo.jpg", 2);

	std::vector<std::string> faces{
		"skybox\\right.jpg",
		"skybox\\left.jpg",
		"skybox\\top.jpg",
		"skybox\\bottom.jpg",
		"skybox\\front.jpg",
		"skybox\\back.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);
	
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER,skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));

	glm::mat4 modelMat;
	modelMat = glm::translate(modelMat, glm::vec3(0.f));
	glm::mat4 viewMat;
	viewMat = camera.getViewMat();
	glm::mat4 projMat;
	projMat = glm::perspective(glm::radians(50.f), 800.f / 600.f, 0.01f, 100.f);
		while (!glfwWindowShouldClose(window)) {
			processInput(window);
			viewMat = camera.getViewMat();
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
			
				shader.use();
				glUniform1i(glGetUniformLocation(shader.ID, "material.diffuse"), 2);
				glUniformMatrix4fv(glGetUniformLocation(shader.ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
				glUniformMatrix4fv(glGetUniformLocation(shader.ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
				glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
				glUniform3f(glGetUniformLocation(shader.ID, "light.Position"), lightPos.x, lightPos.y, lightPos.z);
				glUniform3f(glGetUniformLocation(shader.ID, "light.ambient"), 1.f, 1.f, 1.f);
				glUniform3f(glGetUniformLocation(shader.ID, "light.diffuse"), 1.f, 1.f, 1.f);
				glUniform3f(glGetUniformLocation(shader.ID, "light.specular"), 1.f, 1.f, 1.f);
				glUniform1f(glGetUniformLocation(shader.ID, "light.constant"), 1.f);
				glUniform1f(glGetUniformLocation(shader.ID, "light.linear"), 0.09);
				glUniform1f(glGetUniformLocation(shader.ID, "light.quadratic"), 0.0032);
				lightModel.Draw(renderShader);
			
				glDepthFunc(GL_LEQUAL);
				skyboxShader.use();
				glBindVertexArray(skyboxVAO);
				viewMat = glm::mat4(glm::mat3(camera.getViewMat()));//remove the translation transformation 
				glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
				glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
				
				glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				glBindVertexArray(0);
				glDepthFunc(GL_LESS);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		glfwTerminate();
		return 0;
		
}	
void processInput(GLFWwindow* window) {
	float currFrame = glfwGetTime();
	deltaTime = currFrame - lastFrame;
	lastFrame = currFrame;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
	}
	float camera_speed = 5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.Postion += camera_speed * camera.Forward;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.Postion -= camera_speed * camera.Forward;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.Postion += camera_speed * camera.Right;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.Postion -= camera_speed * camera.Right;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		camera.Postion += camera_speed * camera.UP;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		camera.Postion -= camera_speed * camera.UP;
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		lightPos += camera.Forward*camera_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		lightPos -= camera.Forward * camera_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		lightPos -= camera.Right * camera_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		lightPos += camera.Right * camera_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		lightPos += camera.UP * camera_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		lightPos -= camera.UP * camera_speed;
	}

	}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;
	float sensitive = 0.1f;
	xoffset *= sensitive;
	yoffset *= sensitive;
	camera.processMouseMovement(xoffset, yoffset);
}
void loadTexture(const char*fileName ,unsigned int bound) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int height, width, nrComponets;
	unsigned char* data = stbi_load(fileName, &width, &height, &nrComponets, NULL);
	if (data) {
		GLenum format;
		if (nrComponets == 1)
			format = GL_RED;
		else if (nrComponets == 3)
			format = GL_RGB;
		else if (nrComponets == 4)
			format = GL_RGBA;
		glActiveTexture(GL_TEXTURE0+bound);
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
		std::cout << "failed to load texture: " << fileName<< std::endl;
		stbi_image_free(data);
	}

}
void showMat(glm::mat4 mat) {
	static int i = 0;
	if (i++ >= 8) return;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << mat[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

unsigned int loadCubemap(std::vector<std::string> faces) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "failed to load texture: " << faces[i].c_str() << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return textureID;
}

