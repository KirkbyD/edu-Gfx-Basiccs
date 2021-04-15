#pragma once
#include "cMesh.hpp"
#include "cModelObject.hpp"
#include "cLightObject.hpp"
#include "cComplexObject.hpp"
#include "cFirstPersonCamera.hpp"
#include "cThirdPersonCamera.hpp"
#include <vector>
#include <string>
#include <map>

// returns NULL (0) if we didn't find it.
cModelObject* pFindObjectByFriendlyName(std::string name);
cModelObject* pFindObjectByFriendlyNameMap(std::string name);

#pragma region Main
// These are defined in theMain.cpp
extern std::map<std::string, eShapeTypes> g_mp_String_to_ShapeType;

extern std::map<std::string, cMesh*> mpMesh;
extern std::map<std::string, cModelObject*> mpModel;
extern std::map<std::string, cLightObject*> mpLight;

extern cThirdPersonCamera mainCamera;
extern float deltaTime;	// Time between current frame and last frame
extern float lastFrame; // Time of last frame
//extern glm::vec3 cameraEye;
//extern glm::vec3 cameraTarget;
//extern glm::vec3 upVector;

extern glm::vec3 sexyLightPosition;
extern float sexyLightConstAtten;
extern float sexyLightLinearAtten;
extern float sexyLightQuadraticAtten;

extern float sexyLightSpotInnerAngle;
extern float sexyLightSpotOuterAngle;
#pragma endregion


// This is a "normalized" direction (i.e. the length is 1.0f)
extern glm::vec3 sexyLightSpotDirection;

extern bool bLightDebugSheresOn;

// REMOVE THESE IF THEY ARE NOT REQUIRED TO BE HERE LATER
extern bool isDevCon;
extern bool isLightSelect;
extern bool isModelSelect;
extern bool isComplexSelect;
extern bool isObjCtrl;

// COMMAND STRING FOR THE DEVELOPER CONSOLE
// ONLY FOR THE DEVELOPER CONSOLE, FIND A WAY TO CHANGE THIS (THE GAME OBJECTS VEC)
extern std::vector<cModelObject*> g_vec_pGameObjects;
extern std::vector<cLightObject*> g_vec_pLightObjects;
extern std::vector<cComplexObject*> g_vec_pComplexObjects;
extern std::vector<cMesh*> g_vec_pMeshObjects;
extern std::string cmdstr;
extern int vpGameObjectsCurrentPos;
extern int vpLightObjectsCurrentPos;
extern int vpComplexObjectsCurrentPos;



#pragma region cMouseBindings
extern float mouseLastX;
extern float mouseLastY;
#pragma endregion