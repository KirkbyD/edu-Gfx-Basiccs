#include "cThirdPersonCamera.hpp"

cThirdPersonCamera::cThirdPersonCamera() {
	this->yaw = 0.0f;
	this->pitch = 0.0f;
	this->followDistance = 1.0f;

	this->eye = glm::vec3(0.0f, 0.0f, 0.0f);
	this->front = glm::vec3(0.0f, 0.0f, 1.0f);
	this->upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	this->right = glm::normalize(glm::cross(this->upVector, this->front));
	this->up = glm::cross(this->front, this->right);
	this->viewMatrix = glm::mat4(1.0f);

	this->followObject = NULL;
}

glm::vec3 cThirdPersonCamera::GetEye() {
	return this->eye;
}

glm::vec3 cThirdPersonCamera::GetUpVector() {
	return this->upVector;
}

glm::vec3 cThirdPersonCamera::GetFront() {
	return this->front;
}

glm::vec3 cThirdPersonCamera::GetRight()
{
	return this->right;
}

glm::vec3 cThirdPersonCamera::GetUp()
{
	return up;
}

glm::mat4 cThirdPersonCamera::GetViewMatrix()
{
	this->viewMatrix = glm::lookAt(this->eye,
		this->eye + this->front,
		this->upVector);

	return this->viewMatrix;
}

void cThirdPersonCamera::SetUpVector(glm::vec3 upVector) {
	this->upVector = upVector;
}

void cThirdPersonCamera::SetObject(iGameObject* newFollowTarget)
{
	this->followObject = newFollowTarget;
}

void cThirdPersonCamera::SetFollowDistance(float distanceMultiplier)
{
	this->followDistance = distanceMultiplier;
}

void cThirdPersonCamera::MoveForBack(float speed)
{
	this->eye += this->front * speed;
}

void cThirdPersonCamera::MoveUpDown(float speed)
{
	this->eye += this->up * speed;
}

void cThirdPersonCamera::MoveStrafe(float speed)
{
	this->eye += this->right * speed;
}

void cThirdPersonCamera::Move() {
	this->eye = this->followObject->getPosition();

	this->eye += this->front * -5.0f * followDistance;
	this->eye += this->up * 1.5f * followDistance;
	//this->eye += this->right * -2.0f;
}

void cThirdPersonCamera::MoveFront()
{
	this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->front.y = sin(glm::radians(this->pitch));
	this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->front = glm::normalize(this->front);

	this->right = glm::normalize(glm::cross(this->upVector, this->front));
	this->up = glm::cross(this->front, this->right);
}

void cThirdPersonCamera::OffsetYaw(float offset) {
	this->yaw = glm::mod(this->yaw + offset, 360.0f);
}

void cThirdPersonCamera::OffsetPitch(float offset) {
	this->pitch += offset;
	if (this->pitch > 89.0f)
		this->pitch = 89.0f;
	if (this->pitch < -89.0f)
		this->pitch = -89.0f;
}
