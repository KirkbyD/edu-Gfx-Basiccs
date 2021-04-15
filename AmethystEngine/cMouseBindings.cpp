#include "GLCommon.hpp"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "cCallbacks.hpp"
#include "Globals.hpp"

#include <stdio.h>

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//// Move the sphere to where the camera is and shoot the ball from there...

	//cGameObject* pTheBall = pFindObjectByFriendlyName("Sphere#1");

	//// What's the velocity
	//// Target - eye = direction
	//glm::vec3 direction = glm::normalize(cameraTarget - cameraEye);

	//float speed = 5.0f;

	//pTheBall->velocity = direction * speed;
	//pTheBall->positionXYZ = cameraEye;

	//return;
}


float mouseLastX = 320;
float mouseLastY = 240;

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) 
{
	float xoffset = xpos - mouseLastX;
	float yoffset = mouseLastY - ypos;
	mouseLastX = xpos;
	mouseLastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	mainCamera.OffsetYaw(xoffset);
	mainCamera.OffsetPitch(yoffset);

	mainCamera.MoveFront();
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) { }
