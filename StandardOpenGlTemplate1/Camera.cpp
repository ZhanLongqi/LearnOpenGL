#include "Camera.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
Camera::Camera(glm::vec3 pos, float pitch1, float yaw, glm::vec3 world_up) {
	Postion = pos;
	this->pitch = pitch1;
	this->yaw = yaw;
	this->world_up = world_up;
	updateCameraVectors();
}
void Camera::processMouseMovement(float deltaX, float deltaY) {
	pitch -= deltaY;
	yaw -= deltaX;
	float bound = 89.0f;
	if (pitch > bound)pitch = bound;
	if (pitch < -bound)pitch = -bound;
	if (yaw > 180)yaw -= 360;
	if (yaw < -180)yaw += 360;
	updateCameraVectors();
}
void Camera::updateCameraVectors() {
	Forward.x = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
	Forward.y = glm::sin(glm::radians(pitch));
	Forward.z = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
	Right = glm::cross(Forward, world_up);
	Right = glm::normalize(Right);
	UP = glm::cross(Right, Forward);
	UP = glm::normalize(UP);
}
glm::mat4 Camera::getViewMat() {
	return glm::lookAt(Postion, Postion + Forward, UP);
}