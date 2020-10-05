#pragma once
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtc/type_ptr.hpp"
class Camera
{
public:
	Camera(glm::vec3 pos, float pitch, float paw, glm::vec3 world_up);
	glm::mat4 getViewMat();
	glm::vec3 Postion;
	glm::vec3 Forward;
	glm::vec3 Right;
	glm::vec3 world_up;
	glm::vec3 UP;
	float pitch;
	float yaw;
	void processMouseMovement(float deltaX, float deltaY);
private:
	void updateCameraVectors();
};

