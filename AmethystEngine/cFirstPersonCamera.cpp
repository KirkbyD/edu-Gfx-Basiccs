#include "cFirstPersonCamera.hpp"

cFirstPersonCamera::cFirstPersonCamera() {
	this->yaw = 0.0f;
	this->pitch = 0.0f;

	this->eye = glm::vec3(0.0f, 0.0f, 0.0f);
	this->front = glm::vec3(0.0f, 0.0f, 1.0f);
	this->upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	this->right = glm::normalize(glm::cross(this->upVector, this->front));
	this->up = glm::cross(this->front, this->right);
	this->viewMatrix = glm::mat4(1.0f);
}

glm::vec3 cFirstPersonCamera::GetEye() {
	return this->eye;
}

glm::vec3 cFirstPersonCamera::GetUpVector() {
	return this->upVector;
}

glm::vec3 cFirstPersonCamera::GetFront() {
	return this->front;
}

glm::mat4 cFirstPersonCamera::GetViewMatrix() {
	this->viewMatrix = glm::lookAt(this->eye,
		this->eye + this->front,
		this->upVector);

	return this->viewMatrix;
}

void cFirstPersonCamera::SetEye(glm::vec3 location) {
	this->eye = location;
}

void cFirstPersonCamera::SetUpVector(glm::vec3 upVector) {
	this->upVector = upVector;
}

void cFirstPersonCamera::MoveForBack(float speed) {
	this->eye += this->front * speed;
}

void cFirstPersonCamera::MoveUpDown(float speed) {
	this->eye += this->up * speed;
}

void cFirstPersonCamera::MoveStrafe(float speed) {
	this->eye += this->right * speed;
}

void cFirstPersonCamera::MoveFront() {
	this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->front.y = sin(glm::radians(this->pitch));
	this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->front = glm::normalize(this->front);

	this->right = glm::normalize(glm::cross(this->upVector, this->front));
	this->up = glm::cross(this->front, this->right);
}

void cFirstPersonCamera::OffsetYaw(float offset) {
	//Constrains the yaw to within 360
	this->yaw = glm::mod(this->yaw + offset, 360.0f);
}

void cFirstPersonCamera::OffsetPitch(float offset) {
	this->pitch += offset;
	if (this->pitch > 89.0f)
		this->pitch = 89.0f;
	if (this->pitch < -89.0f)
		this->pitch = -89.0f;
}
