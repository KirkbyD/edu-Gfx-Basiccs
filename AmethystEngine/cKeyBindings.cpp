#include "GLCommon.hpp"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "cCallbacks.hpp"
#include "cCommandManager.hpp"
#include "Globals.hpp"

#include <stdio.h>
#include <iostream>o

bool isShiftKeyDownByAlone(int mods);
bool isCtrlKeyDownByAlone(int mods);
float devSpeed = 180.0f;
float maxSpeed = 2.0;

// ADDITIONALLY CHANGE THE DEVELOPER CONSOLE TO USE A SECONDARY WINDOW WITH THE STUFF FROM OSCARS CLASS
// FUCNTION FOR ACTUALLY ISSUING THE COMMAND (THIS IS ESSENTIALLY THE MEDIATOR)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	cCommandManager* pCmdMngr = cCommandManager::GetCommandManager();
	float accelSpeed = 180.0f * deltaTime;
	float rotationSpeed = 1.0f * deltaTime; //assuming 60 fps, move 3 unit a second
	float devMoveSpeed = devSpeed * deltaTime;

	// BECAUSE COMPLICATED MESSY IF/ELSE CRAP BELOW F KEY TO TURN ON/OFF/TOGGLE LIGHTS IS PLACED HERE
	if (!isDevCon && (key == GLFW_KEY_F && action == GLFW_PRESS)) {
		pCmdMngr->IssueCommand("light.visible toggle");
	}

	// OBJECT SELECTION USING THE ARROW KEYS
	if ((key == GLFW_KEY_RIGHT || (key == GLFW_KEY_RIGHT && isDevCon)) && action == GLFW_PRESS) {
		if (isModelSelect) {
			if (vpGameObjectsCurrentPos == g_vec_pGameObjects.size() - 1) {
				vpGameObjectsCurrentPos = 0;
			}
			else {
				++vpGameObjectsCurrentPos;
			}
			mainCamera.SetObject(g_vec_pGameObjects[vpGameObjectsCurrentPos]);
			if (g_vec_pGameObjects[vpGameObjectsCurrentPos]->scale < 1.0f)
				mainCamera.SetFollowDistance(1.0f);
			else
				mainCamera.SetFollowDistance(g_vec_pGameObjects[vpGameObjectsCurrentPos]->scale);
		}

		else if (isLightSelect) {
			if (vpLightObjectsCurrentPos == g_vec_pLightObjects.size() - 1) {
				vpLightObjectsCurrentPos = 0;
			}
			else {
				++vpLightObjectsCurrentPos;
			}
			mainCamera.SetObject(g_vec_pLightObjects[vpLightObjectsCurrentPos]);
			mainCamera.SetFollowDistance(0);
		}

		else if (isComplexSelect) {
			if (vpComplexObjectsCurrentPos == g_vec_pComplexObjects.size() - 1) {
				vpComplexObjectsCurrentPos = 0;
			}
			else {
				++vpComplexObjectsCurrentPos;
			}
			mainCamera.SetObject(g_vec_pComplexObjects[vpComplexObjectsCurrentPos]);
			mainCamera.SetFollowDistance(g_vec_pComplexObjects[vpComplexObjectsCurrentPos]->GetScale());
		}
	}
	if ((key == GLFW_KEY_LEFT || (key == GLFW_KEY_LEFT && isDevCon)) && action == GLFW_PRESS) {
		if (isModelSelect) {
			if (vpGameObjectsCurrentPos == 0) {
				vpGameObjectsCurrentPos = g_vec_pGameObjects.size() - 1;
			}
			else {
				--vpGameObjectsCurrentPos;
			}
			mainCamera.SetObject(g_vec_pGameObjects[vpGameObjectsCurrentPos]);
			if (g_vec_pGameObjects[vpGameObjectsCurrentPos]->scale < 1.0f)
				mainCamera.SetFollowDistance(1.0f);
			else
				mainCamera.SetFollowDistance(g_vec_pGameObjects[vpGameObjectsCurrentPos]->scale);
		}

		else if (isLightSelect) {
			if (vpLightObjectsCurrentPos == 0) {
				vpLightObjectsCurrentPos = g_vec_pLightObjects.size() - 1;
			}
			else {
				--vpLightObjectsCurrentPos;
			}
			mainCamera.SetObject(g_vec_pLightObjects[vpLightObjectsCurrentPos]);
			mainCamera.SetFollowDistance(0);
		}

		else if (isComplexSelect) {
			if (vpComplexObjectsCurrentPos == g_vec_pComplexObjects.size() - 1) {
				vpComplexObjectsCurrentPos = 0;
			}
			else {
				++vpComplexObjectsCurrentPos;
			}
			mainCamera.SetObject(g_vec_pComplexObjects[vpComplexObjectsCurrentPos]);
			mainCamera.SetFollowDistance(g_vec_pComplexObjects[vpComplexObjectsCurrentPos]->GetScale());
		}
	}

	// DEVELOPER CONSOLE ACTIVATION CHECK
	if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS) {
		if (!isDevCon) {
			isDevCon = true;
			glfwSetCharCallback(window, character_callback);
		}
		else {
			isDevCon = false;
			glfwSetCharCallback(window, NULL);
		}
	}

	// DEVELOPER CONSOLE ENTER CHARACTER
	if (isDevCon && key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		pCmdMngr->IssueCommand(cmdstr);
		cmdstr = "";
	}

	// DEVELOPER CONSOLE DELETE CHARACTER
	if (isDevCon && key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (!cmdstr.empty()) {
			cmdstr.pop_back();
		}
	}
	
	if (!isDevCon && !isShiftKeyDownByAlone(mods) && !isCtrlKeyDownByAlone(mods) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		// SWITCH THE + AND - FROM THE CAMERA SPEEDS TO THE OPPOSITE TO MAKE THE DIRECTION OF THE ROTATION NORMAL FEELING
		// Move the camera (A & D for left and right, along the x axis)
		// Working on creation of a first person camera with mouse motion.
		if (isModelSelect) {
			if (key == GLFW_KEY_A) {
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->velocity.x += mainCamera.GetRight().x * accelSpeed;
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->velocity.z += mainCamera.GetRight().z * accelSpeed;			
			}
			if (key == GLFW_KEY_D) {
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->velocity.x -= mainCamera.GetRight().x * accelSpeed;
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->velocity.z -= mainCamera.GetRight().z * accelSpeed;
			}
			if (key == GLFW_KEY_SPACE) {
				//clamp 'jumping'
				if (g_vec_pGameObjects[vpGameObjectsCurrentPos]->velocity.y < 1.0f
					&& g_vec_pGameObjects[vpGameObjectsCurrentPos]->positionXYZ.y < 1.5 * g_vec_pGameObjects[vpGameObjectsCurrentPos]->scale)
					g_vec_pGameObjects[vpGameObjectsCurrentPos]->velocity.y += 10.0f;
			}
			if (key == GLFW_KEY_W) {
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->velocity.x += mainCamera.GetFront().x * accelSpeed;
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->velocity.z += mainCamera.GetFront().z * accelSpeed;
			}
			if (key == GLFW_KEY_S) {
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->velocity.x -= mainCamera.GetFront().x * accelSpeed;
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->velocity.z -= mainCamera.GetFront().z * accelSpeed;
			}

			//clamps x y movement?
		/*	if (g_vec_pGameObjects[vpGameObjectsCurrentPos]->velocity.x > maxSpeed)
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->velocity.x = maxSpeed;
			else if (g_vec_pGameObjects[vpGameObjectsCurrentPos]->velocity.x < -maxSpeed)
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->velocity.x = -maxSpeed;

			if (g_vec_pGameObjects[vpGameObjectsCurrentPos]->velocity.z > maxSpeed)
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->velocity.z = maxSpeed;
			else if (g_vec_pGameObjects[vpGameObjectsCurrentPos]->velocity.z > -maxSpeed)
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->velocity.z = -maxSpeed;*/
			
		}
		else if (isComplexSelect) {
			glm::vec3 velocity = g_vec_pComplexObjects[vpGameObjectsCurrentPos]->GetVelocity();
			if (key == GLFW_KEY_A) {
				g_vec_pComplexObjects[vpGameObjectsCurrentPos]->SetVelocity(glm::vec3(	velocity.x + mainCamera.GetRight().x * accelSpeed,
																						velocity.y,
																						velocity.z + mainCamera.GetRight().z * accelSpeed));
			}
			if (key == GLFW_KEY_D) {
				g_vec_pComplexObjects[vpGameObjectsCurrentPos]->SetVelocity(glm::vec3(	velocity.x - mainCamera.GetRight().x * accelSpeed,
																						velocity.y,
																						velocity.z - mainCamera.GetRight().z * accelSpeed));
			}
			if (key == GLFW_KEY_SPACE)
				g_vec_pComplexObjects[vpGameObjectsCurrentPos]->SetVelocity(glm::vec3(	velocity.x,
																						velocity.y += 20.0f,
																						velocity.z));
			if (key == GLFW_KEY_W) {
				g_vec_pComplexObjects[vpGameObjectsCurrentPos]->SetVelocity(glm::vec3(	velocity.x + mainCamera.GetFront().x * accelSpeed,
																						velocity.y,
																						velocity.z + mainCamera.GetFront().z * accelSpeed));
			}
			if (key == GLFW_KEY_S) {
				g_vec_pComplexObjects[vpGameObjectsCurrentPos]->SetVelocity(glm::vec3(	velocity.x - mainCamera.GetFront().x * accelSpeed,
																						velocity.y,
																						velocity.z - mainCamera.GetFront().z * accelSpeed));
			}
		}

		//if (key == GLFW_KEY_B) {
		//	// Shoot a bullet from the pirate ship
		//	// Find the pirate ship...
		//	// returns NULL (0) if we didn't find it.
		//	cGameObject* pShip = pFindObjectByFriendlyNameMap("PirateShip");
		//	// Maybe check to see if it returned something... 

		//	// Find the sphere#2
		//	cGameObject* pBall = pFindObjectByFriendlyNameMap("Sphere#2");

		//	// Set the location velocity for sphere#2
		//	pBall->positionXYZ = pShip->positionXYZ;
		//	pBall->inverseMass = 1.0f;		// So it's updated
		//	// 20.0 units "to the right"
		//	// 30.0 units "up"
		//	pBall->velocity = glm::vec3(15.0f, 20.0f, 0.0f);
		//	pBall->accel = glm::vec3(0.0f, 0.0f, 0.0f);
		//	pBall->diffuseColour = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		//}//if ( key == GLFW_KEY_B )
	}

#pragma region LightCommentS
	// THIS NEEDS TO CHANGE SIMILAR TO THE OBJECT SELECTION PORTION OF THE DEVELOPER CONSOLE FUNCTIONALITY
	// THIS WILL ALSO BE INCORPERATED INTO THE MEDIATOR PATTERN
	//if (isShiftKeyDownByAlone(mods)) {
	//	// move the light
	//	if (key == GLFW_KEY_A)
	//		sexyLightPosition.x -= CAMERASPEED;		// Move the camera -0.01f units

	//	if (key == GLFW_KEY_D)
	//		sexyLightPosition.x += CAMERASPEED;		// Move the camera +0.01f units

	//	// Move the camera (Q & E for up and down, along the y axis)
	//	if (key == GLFW_KEY_Q)
	//		sexyLightPosition.y -= CAMERASPEED;		// Move the camera -0.01f units

	//	if (key == GLFW_KEY_E)
	//		sexyLightPosition.y += CAMERASPEED;		// Move the camera +0.01f units

	//	// Move the camera (W & S for towards and away, along the z axis)
	//	if (key == GLFW_KEY_W)
	//		sexyLightPosition.z -= CAMERASPEED;		// Move the camera -0.01f units

	//	if (key == GLFW_KEY_S)
	//		sexyLightPosition.z += CAMERASPEED;		// Move the camera +0.01f units




	//	if (key == GLFW_KEY_9)
	//		bLightDebugSheresOn = false;

	//	if (key == GLFW_KEY_0)
	//		bLightDebugSheresOn = true;
	//}//if (isShiftKeyDownByAlone(mods))
#pragma endregion

	if (isDevCon && !isShiftKeyDownByAlone(mods) && isCtrlKeyDownByAlone(mods) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		// Position
		if (key == GLFW_KEY_PERIOD) {
			devSpeed *= 1.1;
		}
		if (key == GLFW_KEY_COMMA) {
			devSpeed *= 0.9;
		}
		if (isModelSelect) {
			if (key == GLFW_KEY_A)
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->positionXYZ += mainCamera.GetRight() * devMoveSpeed;
			if (key == GLFW_KEY_D) 
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->positionXYZ -= mainCamera.GetRight() * devMoveSpeed;
			if (key == GLFW_KEY_Q)
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->positionXYZ += mainCamera.GetUp() * devMoveSpeed;
			if (key == GLFW_KEY_E)
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->positionXYZ -= mainCamera.GetUp() * devMoveSpeed;
			if (key == GLFW_KEY_W)
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->positionXYZ += mainCamera.GetFront() * devMoveSpeed;
			if (key == GLFW_KEY_S)
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->positionXYZ -= mainCamera.GetFront() * devMoveSpeed;
			if (key == GLFW_KEY_R)
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->rotationXYZ.y += 0.02;
			if (key == GLFW_KEY_T)
				g_vec_pGameObjects[vpGameObjectsCurrentPos]->rotationXYZ.y -= 0.02;
		}
		if (isLightSelect) {
			if (key == GLFW_KEY_A) {
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->position.x += mainCamera.GetRight().x * devMoveSpeed;
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->position.y += mainCamera.GetRight().y * devMoveSpeed;
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->position.z += mainCamera.GetRight().z * devMoveSpeed;
			}
			if (key == GLFW_KEY_D) {
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->position.x -= mainCamera.GetRight().x * devMoveSpeed;
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->position.y -= mainCamera.GetRight().y * devMoveSpeed;
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->position.z -= mainCamera.GetRight().z * devMoveSpeed;
			}
			if (key == GLFW_KEY_Q) {
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->position.x += mainCamera.GetUp().x * devMoveSpeed;
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->position.y += mainCamera.GetUp().y * devMoveSpeed;
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->position.z += mainCamera.GetUp().z * devMoveSpeed;
			}
			if (key == GLFW_KEY_E) {
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->position.x -= mainCamera.GetUp().x * devMoveSpeed;
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->position.y -= mainCamera.GetUp().y * devMoveSpeed;
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->position.z -= mainCamera.GetUp().z * devMoveSpeed;
			}
			if (key == GLFW_KEY_W) {
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->position.x += mainCamera.GetFront().x * devMoveSpeed;
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->position.y += mainCamera.GetFront().y * devMoveSpeed;
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->position.z += mainCamera.GetFront().z * devMoveSpeed;
			}
			if (key == GLFW_KEY_S) {
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->position.x -= mainCamera.GetFront().x * devMoveSpeed;
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->position.y -= mainCamera.GetFront().y * devMoveSpeed;
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->position.z -= mainCamera.GetFront().z * devMoveSpeed;
			}
			if (key == GLFW_KEY_1)
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->atten.x *= 0.99f;			// 99% of what it was
			if (key == GLFW_KEY_2)
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->atten.x *= 1.01f;			// 1% more of what it was
			if (key == GLFW_KEY_3)
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->atten.y *= 0.99f;			// 99% of what it was
			if (key == GLFW_KEY_4)
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->atten.y *= 1.01f;			// 1% more of what it was
			if (key == GLFW_KEY_5)
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->atten.z *= 0.99f;			// 99% of what it was
			if (key == GLFW_KEY_6)
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->atten.z *= 1.01f;			// 1% more of what it was
			if (key == GLFW_KEY_V)
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->param1.y -= 0.1f;
			if (key == GLFW_KEY_B)
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->param1.y += 0.1f;
			if (key == GLFW_KEY_N)
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->param1.z -= 0.1f;
			if (key == GLFW_KEY_M)
				g_vec_pLightObjects[vpLightObjectsCurrentPos]->param1.z += 0.1f;
		}
		if (isComplexSelect) {
			if (key == GLFW_KEY_A)
				g_vec_pComplexObjects[vpComplexObjectsCurrentPos]->SetPosition(	g_vec_pComplexObjects[vpComplexObjectsCurrentPos]->GetPosition()
																				+ mainCamera.GetRight() * devMoveSpeed);
			if (key == GLFW_KEY_D)
				g_vec_pComplexObjects[vpComplexObjectsCurrentPos]->SetPosition(	g_vec_pComplexObjects[vpComplexObjectsCurrentPos]->GetPosition()
																				- mainCamera.GetRight() * devMoveSpeed);
			if (key == GLFW_KEY_Q)
				g_vec_pComplexObjects[vpComplexObjectsCurrentPos]->SetPosition(	g_vec_pComplexObjects[vpComplexObjectsCurrentPos]->GetPosition()
																				+ mainCamera.GetUp() * devMoveSpeed);
			if (key == GLFW_KEY_E)
				g_vec_pComplexObjects[vpComplexObjectsCurrentPos]->SetPosition(	g_vec_pComplexObjects[vpComplexObjectsCurrentPos]->GetPosition()
																				- mainCamera.GetUp() * devMoveSpeed);
			if (key == GLFW_KEY_W)
				g_vec_pComplexObjects[vpComplexObjectsCurrentPos]->SetPosition(	g_vec_pComplexObjects[vpComplexObjectsCurrentPos]->GetPosition()
																				+ mainCamera.GetFront() * devMoveSpeed);
			if (key == GLFW_KEY_S)
				g_vec_pComplexObjects[vpComplexObjectsCurrentPos]->SetPosition(	g_vec_pComplexObjects[vpComplexObjectsCurrentPos]->GetPosition()
																				- mainCamera.GetFront() * devMoveSpeed);
		}
	}

	// Swap iGameObject* selection vectors
	if (key == GLFW_KEY_LEFT_BRACKET) {
		if (g_vec_pGameObjects.size() != 0) {
			isModelSelect = true;
			isLightSelect = false;
			isComplexSelect = false;

			mainCamera.SetObject(g_vec_pGameObjects[vpGameObjectsCurrentPos]);
			mainCamera.SetFollowDistance(g_vec_pGameObjects[vpGameObjectsCurrentPos]->scale);
		}
	}
	if (key == GLFW_KEY_RIGHT_BRACKET) {
		if (g_vec_pLightObjects.size() != 0) {
			isModelSelect = false;
			isLightSelect = true;
			isComplexSelect = false;

			mainCamera.SetObject(g_vec_pLightObjects[vpLightObjectsCurrentPos]);
			mainCamera.SetFollowDistance(0);
		}
	}
	if (key == GLFW_KEY_BACKSLASH) {
		if (g_vec_pComplexObjects.size() != 0) {
			isModelSelect = false;
			isLightSelect = false;
			isComplexSelect = true;
			mainCamera.SetObject(g_vec_pComplexObjects[vpComplexObjectsCurrentPos]);
			mainCamera.SetFollowDistance(g_vec_pGameObjects[vpComplexObjectsCurrentPos]->scale);
		}
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

bool isShiftKeyDownByAlone(int mods) {
	if (mods == GLFW_MOD_SHIFT)
		return true;
	return false;
}

bool isCtrlKeyDownByAlone(int mods) {
	if (mods == GLFW_MOD_CONTROL)
		return true;
	return false;
}

void character_callback(GLFWwindow* window, unsigned codepoint) {
	// STUFF FOR THE DEVELOPER CONSOLE
	if ((char)codepoint != '`')
		cmdstr.push_back((char)codepoint);
}