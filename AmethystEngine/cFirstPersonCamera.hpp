#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

//#include "iCamera.hpp"

class cFirstPersonCamera {
private:
	float yaw;	
	float pitch;
	glm::vec3 eye;			//defaults  (0.0f, 0.0f, 0.0f)
	glm::vec3 upVector;		//defaults	(0.0f, 1.0f, 0.0f)
	glm::vec3 front;		//defaults  (0.0f, 0.0f, 1.0f)
	glm::vec3 right;
	glm::vec3 up;
	glm::mat4 viewMatrix;

public:
	cFirstPersonCamera();

	glm::vec3 GetEye();
	glm::vec3 GetUpVector();
	glm::vec3 GetFront();
	glm::mat4 GetViewMatrix();

	void SetEye(glm::vec3 location);
	void SetUpVector(glm::vec3 upVector);
	void MoveForBack(float speed);
	void MoveUpDown(float speed);
	void MoveStrafe(float speed);
	void MoveFront();

	void OffsetYaw(float offset);
	void OffsetPitch(float offset);
};