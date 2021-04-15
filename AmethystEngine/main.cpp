#pragma region Includes
#include "GLCommon.hpp"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include "cModelLoader.hpp"			
#include "cVAOManager.hpp"
#include "cModelObject.hpp"
#include "cShaderManager.hpp"
#include <sstream>
#include <limits.h>
#include <float.h>
#include "cPhysics.hpp"
#include "DebugRenderer/cDebugRenderer.hpp"
#include "cLightHelper.hpp"
#include "cLightManager.hpp"
#include "cFileManager.hpp"
#include "cError.hpp"
#include "cCallbacks.hpp"
#include "cFirstPersonCamera.hpp"
#include "cThirdPersonCamera.hpp"
#include "Globals.hpp"
#include "cModelManager.hpp"
#include "cCommandManager.hpp"
#include "cMediator.hpp"
#include "cComplexObject.hpp"
#pragma endregion

void DrawObject(glm::mat4 m, cModelObject* pCurrentObject, GLint shaderProgID, cVAOManager* pVAOManager);

std::map<std::string, cMesh*> mpMesh;
std::map<std::string, cLightObject*> mpLight;
std::map<std::string, cModelObject*> mpModel;
//cFirstPersonCamera mainCamera;
cThirdPersonCamera mainCamera;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool bLightDebugSheresOn = true;

int vpGameObjectsCurrentPos = 0;
int vpLightObjectsCurrentPos = 0;
int vpComplexObjectsCurrentPos = 0;

// VARIABLES FOR SELECTING AND CONTROLING VARIOUS ASPECTS OF THE ENGINE AT RUNTIME
//		isDevCons		=		Is Developer Console Enabled?
//		isLightSelect	=		Is Lights Selectable Enabled?
//		isModelSelect	=		Is Models Selectable Enabled?
//		isObjCtrl		=		Is Objects Controllable Enabled?
bool isDevCon = false;
bool isModelSelect = false;
bool isLightSelect = false;
bool isComplexSelect = false;
bool isObjCtrl = false;

std::string cmdstr;

// Load up my "scene"  (now global)
// OBJECT VECTORS
std::vector<cModelObject*> g_vec_pGameObjects;
std::vector<cLightObject*> g_vec_pLightObjects;
std::vector<cComplexObject*> g_vec_pComplexObjects;
std::vector<cMesh*> g_vec_pMeshObjects;

// MAPS
//std::map<std::string /*FriendlyName*/, cModelObject*> g_map_GameObjectsByFriendlyName;


// returns NULL (0) if we didn't find it.
cModelObject* pFindObjectByFriendlyName(std::string name);
cModelObject* pFindObjectByFriendlyNameMap(std::string name);


// MOVED USER INPUT TO AN EXTERNAL FILE.

// command to target different object types...
// model = meshes
// lighting = lights
// ...
// command = target parm ||| target lighting as an example
// commands should handle no parms, 1 parm, or two parms.

std::map<std::string, eShapeTypes> g_mp_String_to_ShapeType;

void PopulateShapeTypeMap() {
	g_mp_String_to_ShapeType["CUBE"] = eShapeTypes::CUBE;
	g_mp_String_to_ShapeType["MESH"] = eShapeTypes::MESH;
	g_mp_String_to_ShapeType["PLANE"] = eShapeTypes::PLANE;
	g_mp_String_to_ShapeType["SPHERE"] = eShapeTypes::SPHERE;
	g_mp_String_to_ShapeType["UNKNOWN"] = eShapeTypes::UNKNOWN;
}



int main() {
	PopulateShapeTypeMap();

	cError errmngr;
	cFileManager* pFileManager = cFileManager::GetFileManager();
	cLightManager* pLightManager = cLightManager::GetLightManager();
	cModelManager* pModelManager = cModelManager::GetModelManager();
	cCommandManager* pCommandManager = cCommandManager::GetCommandManager();

	cMediator* pMediator = new cMediator();
	pMediator->ConnectEndpointClients();

	GLFWwindow* window;

	std::map<std::string, cShaderManager::cShader> mpVertexShader;
	std::map<std::string, cShaderManager::cShader> mpFragmentShader;



	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);



	cDebugRenderer* pDebugRenderer = new cDebugRenderer();
	pDebugRenderer->initialize();

	cModelLoader* pTheModelLoader = new cModelLoader();	// Heap
	cShaderManager* pTheShaderManager = new cShaderManager();

	pFileManager->LoadShaders(pFileManager->GetShadersFile(), mpVertexShader, mpFragmentShader);

	// change LoadShaders to bool and return true or false based off of success, then change below to return -1 if false.
	// error will display from the LoadShaders function.
	if (!pTheShaderManager->createProgramFromFile("SimpleShader", mpVertexShader["exampleVertexShader01"], mpFragmentShader["exampleFragmentShader01"])) {
		errmngr.DisplayError("Main", __func__, "shader_compile_failure", "Could not compile shader", pTheShaderManager->getLastError());
		return -1;
	}

	GLuint shaderProgID = pTheShaderManager->getIDFromFriendlyName("SimpleShader");

	// Create a VAO Manager...
	cVAOManager* pTheVAOManager = new cVAOManager();



	// move this configuration into a seperate file that loads from a configuration file.
	// Note, the "filename" here is really the "model name" that we can look up later (i.e. it doesn't have to be the file name)

	pFileManager->LoadModelsOntoGPU(pTheModelLoader, pTheVAOManager, shaderProgID, pFileManager->GetMeshFile(), mpMesh);

	//move the physics stuff into a physics.ini so that it can be loaded in the physics engine instead of the graphics engine portion of the game engine file loading.
	// At this point, the model is loaded into the GPU

	// make a call to the filemanager to create the gameobjects from the loaded file.
	//pFileManager->CreateModelsFromData(g_vec_pGameObjects, "models/modelsTest", g_mp_String_to_ShapeType);
	pFileManager->BuildObjects(pFileManager->GetModelFile());
	
	glEnable(GL_DEPTH);			// Write to the depth buffer
	glEnable(GL_DEPTH_TEST);	// Test with buffer when drawing


	cPhysics* pPhsyics = new cPhysics();
	cLightHelper* pLightHelper = new cLightHelper();

	//pFileManager->CreateLights("lights/lights", g_vec_pLightObjects, mpLight);
	pFileManager->BuildObjects(pFileManager->GetLightsFile());

	//pFileManager->BuildObjects(pFileManager->GetComplexObjFile());

	//mainCamera.SetEye(glm::vec3(0.0, 30.0, -100.0));
	if (g_vec_pComplexObjects.size() != 0) {
		mainCamera.SetObject(g_vec_pComplexObjects[0]);
		isComplexSelect = true;
	}
	else if (g_vec_pGameObjects.size() != 0) {
		mainCamera.SetObject(g_vec_pGameObjects[0]);
		isModelSelect = true;
	}
	else {
		//TODO use freecam.
	}


	while (!glfwWindowShouldClose(window)) {
		float ratio;
		int width, height;
		glm::mat4 p, v;

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		// Projection matrix
		p = glm::perspective(0.6f,		// FOV
			ratio,			// Aspect ratio
			0.1f,			// Near clipping plane
			1000.0f);		// Far clipping plane

		// View matrix
		mainCamera.Move();
		v = mainCamera.GetViewMatrix();

		glViewport(0, 0, width, height);

		// Clear both the colour buffer (what we see) and the 
		//  depth (or z) buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// move to a lighting file and call load lights or something of the like.
		// move to a light manager class..
		pLightManager->LoadLightsToGPU(g_vec_pLightObjects, shaderProgID);

		// Also set the position of my "eye" (the camera)
		GLint eyeLocation_UL = glGetUniformLocation(shaderProgID, "eyeLocation");

		glUniform4f(eyeLocation_UL,
			mainCamera.GetEye().x,
			mainCamera.GetEye().y,
			mainCamera.GetEye().z,
			1.0f);

		// DEBUG TITLE CURRENT OBJECT SELECTED
		if (isComplexSelect)
			glfwSetWindowTitle(window, g_vec_pComplexObjects[vpComplexObjectsCurrentPos]->friendlyName.c_str());
		else if (isModelSelect)
			glfwSetWindowTitle(window, g_vec_pGameObjects[vpGameObjectsCurrentPos]->friendlyName.c_str());
		else if (isLightSelect)
			glfwSetWindowTitle(window, g_vec_pLightObjects[vpLightObjectsCurrentPos]->friendlyName.c_str());


		GLint matView_UL = glGetUniformLocation(shaderProgID, "matView");
		GLint matProj_UL = glGetUniformLocation(shaderProgID, "matProj");

		glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
		glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));


		// **************************************************
		// **************************************************
		// Loop to draw everything in the scene

		// move into a render function / class
		for (int index = 0; index != ::g_vec_pGameObjects.size(); index++) {
			glm::mat4 matModel = glm::mat4(1.0f);
			cModelObject* pCurrentObject = ::g_vec_pGameObjects[index];
			if (pCurrentObject->isVisible) { DrawObject(matModel, pCurrentObject, shaderProgID, pTheVAOManager); }
		}//for (int index...

		// **************************************************
		//	Pause physics updates if the dev console is open.
		if (!isDevCon) {
			//GRAPHICS UPDATE LIGHT TARGET to model 0
			glm::vec3 lightDirection = glm::normalize(g_vec_pGameObjects[0]->positionXYZ - g_vec_pLightObjects[0]->getPosition());
			g_vec_pLightObjects[0]->direction = glm::vec4(lightDirection, 1.0f);
			
			//THIS IS HOW YOU FLASHLIGHT
			/*g_vec_pLightObjects[0]->direction = glm::vec4(mainCamera.GetFront(), 0.0f);
			g_vec_pLightObjects[0]->position = glm::vec4(mainCamera.GetEye(), 0.0f);*/


			// Update the objects through physics
			pPhsyics->IntegrationStep(::g_vec_pGameObjects, deltaTime);
			
			pPhsyics->TestForCollisions(::g_vec_pGameObjects);

			// Let's draw all the closest points to the sphere
			// on the terrain mesh....
			// 
			// For each triangle in the terrain mesh:
			// - Run ClosestPointToTriangle
			// - Place the debug sphere "there"
			// - Draw it.
		}

		//{// Draw closest point
		//	glm::mat4 matModel = glm::mat4(1.0f);
		//	pFindObjectByFriendlyName("debug_sphere")->positionXYZ = closestPoint;
		//	pFindObjectByFriendlyName("debug_sphere")->scale = 3.0f;
		//	pFindObjectByFriendlyName("debug_sphere")->debugColour = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
		//	pFindObjectByFriendlyName("debug_sphere")->isWireframe = true;
		//	DrawObject(matModel, pFindObjectByFriendlyName("debug_sphere"), shaderProgID, pTheVAOManager);
		//}

		//// How far did we penetrate the surface?
		//glm::vec3 CentreToClosestPoint = pFindObjectByFriendlyName("Sphere#1")->positionXYZ - closestPoint;

		//// Direction that ball is going is normalized velocity
		//glm::vec3 directionBall = glm::normalize(pFindObjectByFriendlyName("Sphere#1")->velocity);	// 1.0f

		//// Calcualte direction to move it back the way it came from
		//glm::vec3 oppositeDirection = -directionBall;				// 1.0f

		//float distanceToClosestPoint = glm::length(CentreToClosestPoint);


		 /*move into a debug file for rendering this stuff
		 this must target and apply to whichever lights one wishes and thus must be moved to an external file
		 function to addLightDebugSpheres??? in cLightManager???*/
		//if (bLightDebugSheresOn) {
		//	{// Draw where the light is at
		//		glm::mat4 matModel = glm::mat4(1.0f);
		//		pDebugSphere->positionXYZ = sexyLightPosition;
		//		pDebugSphere->scale = 0.5f;
		//		pDebugSphere->debugColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		//		pDebugSphere->isWireframe = true;
		//		DrawObject(matModel, pDebugSphere,
		//			shaderProgID, pTheVAOManager);
		//	}
		//	// Draw spheres to represent the attenuation...
		//	{   // Draw a sphere at 1% brightness
		//		glm::mat4 matModel = glm::mat4(1.0f);
		//		pDebugSphere->positionXYZ = sexyLightPosition;
		//		float sphereSize = pLightHelper->calcApproxDistFromAtten(
		//			0.01f,		// 1% brightness (essentially black)
		//			0.001f,		// Within 0.1%  
		//			100000.0f,	// Will quit when it's at this distance
		//			sexyLightConstAtten,
		//			sexyLightLinearAtten,
		//			sexyLightQuadraticAtten);
		//		pDebugSphere->scale = sphereSize;
		//		pDebugSphere->debugColour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		//		pDebugSphere->isWireframe = true;
		//		DrawObject(matModel, pDebugSphere,
		//			shaderProgID, pTheVAOManager);
		//	}
		//	{   // Draw a sphere at 25% brightness
		//		glm::mat4 matModel = glm::mat4(1.0f);
		//		pDebugSphere->positionXYZ = sexyLightPosition;
		//		float sphereSize = pLightHelper->calcApproxDistFromAtten(
		//			0.25f,		// 1% brightness (essentially black)
		//			0.001f,		// Within 0.1%  
		//			100000.0f,	// Will quit when it's at this distance
		//			sexyLightConstAtten,
		//			sexyLightLinearAtten,
		//			sexyLightQuadraticAtten);
		//		pDebugSphere->scale = sphereSize;
		//		pDebugSphere->debugColour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		//		pDebugSphere->isWireframe = true;
		//		DrawObject(matModel, pDebugSphere,
		//			shaderProgID, pTheVAOManager);
		//	}
		//	{   // Draw a sphere at 50% brightness
		//		glm::mat4 matModel = glm::mat4(1.0f);
		//		pDebugSphere->positionXYZ = sexyLightPosition;
		//		float sphereSize = pLightHelper->calcApproxDistFromAtten(
		//			0.50f,		// 1% brightness (essentially black)
		//			0.001f,		// Within 0.1%  
		//			100000.0f,	// Will quit when it's at this distance
		//			sexyLightConstAtten,
		//			sexyLightLinearAtten,
		//			sexyLightQuadraticAtten);
		//		pDebugSphere->scale = sphereSize;
		//		pDebugSphere->debugColour = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
		//		pDebugSphere->isWireframe = true;
		//		DrawObject(matModel, pDebugSphere,
		//			shaderProgID, pTheVAOManager);
		//	}
		//	{   // Draw a sphere at 75% brightness
		//		glm::mat4 matModel = glm::mat4(1.0f);
		//		pDebugSphere->positionXYZ = sexyLightPosition;
		//		float sphereSize = pLightHelper->calcApproxDistFromAtten(
		//			0.75f,		// 1% brightness (essentially black)
		//			0.001f,		// Within 0.1%  
		//			100000.0f,	// Will quit when it's at this distance
		//			sexyLightConstAtten,
		//			sexyLightLinearAtten,
		//			sexyLightQuadraticAtten);
		//		pDebugSphere->scale = sphereSize;
		//		pDebugSphere->debugColour = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		//		pDebugSphere->isWireframe = true;
		//		DrawObject(matModel, pDebugSphere,
		//			shaderProgID, pTheVAOManager);
		//	}
		//	{   // Draw a sphere at 95% brightness
		//		glm::mat4 matModel = glm::mat4(1.0f);
		//		pDebugSphere->positionXYZ = sexyLightPosition;
		//		float sphereSize = pLightHelper->calcApproxDistFromAtten(
		//			0.95f,		// 1% brightness (essentially black)
		//			0.001f,		// Within 0.1%  
		//			100000.0f,	// Will quit when it's at this distance
		//			sexyLightConstAtten,
		//			sexyLightLinearAtten,
		//			sexyLightQuadraticAtten);
		//		pDebugSphere->scale = sphereSize;
		//		pDebugSphere->debugColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		//		pDebugSphere->isWireframe = true;
		//		DrawObject(matModel, pDebugSphere,
		//			shaderProgID, pTheVAOManager);
		//	}
		//}// if (bLightDebugSheresOn) 

		// **************************************************

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	// Delete everything
	delete pTheModelLoader;
	delete pDebugRenderer;
	delete pLightHelper;
	exit(EXIT_SUCCESS);
}



void DrawObject(glm::mat4 m, cModelObject* pCurrentObject, GLint shaderProgID, cVAOManager* pVAOManager) {
	m = glm::mat4(1.0f);


	// ******* TRANSLATION TRANSFORM *********
	glm::mat4 matTrans
		= glm::translate(glm::mat4(1.0f),
			glm::vec3(pCurrentObject->positionXYZ.x,
				pCurrentObject->positionXYZ.y,
				pCurrentObject->positionXYZ.z));
	m = m * matTrans;
	// ******* TRANSLATION TRANSFORM *********



	// ******* ROTATION TRANSFORM *********
	//mat4x4_rotate_Z(m, m, (float) glfwGetTime());
	glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
		pCurrentObject->rotationXYZ.z,					// Angle 
		glm::vec3(0.0f, 0.0f, 1.0f));
	m = m * rotateZ;

	glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
		pCurrentObject->rotationXYZ.y,	//(float)glfwGetTime(),					// Angle 
		glm::vec3(0.0f, 1.0f, 0.0f));
	m = m * rotateY;

	glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
		pCurrentObject->rotationXYZ.x,	// (float)glfwGetTime(),					// Angle 
		glm::vec3(1.0f, 0.0f, 0.0f));
	m = m * rotateX;
	// ******* ROTATION TRANSFORM *********



	// ******* SCALE TRANSFORM *********
	glm::mat4 scale = glm::scale(glm::mat4(1.0f),
		glm::vec3(pCurrentObject->scale,
			pCurrentObject->scale,
			pCurrentObject->scale));
	m = m * scale;
	// ******* SCALE TRANSFORM *********


	// Choose which shader to use
	//glUseProgram(program);
	glUseProgram(shaderProgID);



	GLint matModel_UL = glGetUniformLocation(shaderProgID, "matModel");

	glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(m));



	// Find the location of the uniform variable newColour
	GLint newColour_location = glGetUniformLocation(shaderProgID, "newColour");

	glUniform3f(newColour_location,
		pCurrentObject->objectColourRGBA.r,
		pCurrentObject->objectColourRGBA.g,
		pCurrentObject->objectColourRGBA.b);


	GLint diffuseColour_UL = glGetUniformLocation(shaderProgID, "diffuseColour");
	glUniform4f(diffuseColour_UL,
		pCurrentObject->objectColourRGBA.r,
		pCurrentObject->objectColourRGBA.g,
		pCurrentObject->objectColourRGBA.b,
		pCurrentObject->objectColourRGBA.a);	// 

	GLint specularColour_UL = glGetUniformLocation(shaderProgID, "specularColour");
	glUniform4f(specularColour_UL,
		pCurrentObject->specularColour.r,
		pCurrentObject->specularColour.g,
		pCurrentObject->specularColour.b,
		pCurrentObject->specularColour.a);
	//glUniform4f(specularColour_UL,
	//	1.0f,	// R
	//	1.0f,	// G
	//	1.0f,	// B
	//	1000.0f);	// Specular "power" (how shinny the object is)
	//				// 1.0 to really big (10000.0f)

	GLint debugColour_UL = glGetUniformLocation(shaderProgID, "debugColour");
	GLint bDoNotLight_UL = glGetUniformLocation(shaderProgID, "bDoNotLight");

	if (pCurrentObject->isWireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// LINES
		glUniform4f(debugColour_UL,
			pCurrentObject->debugColour.r,
			pCurrentObject->debugColour.g,
			pCurrentObject->debugColour.b,
			pCurrentObject->debugColour.a);
		glUniform1f(bDoNotLight_UL, (float)GL_TRUE);
	}
	else {	// Regular object (lit and not wireframe)
		glUniform1f(bDoNotLight_UL, (float)GL_FALSE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID
	}
	//glPointSize(15.0f);

	if (pCurrentObject->disableDepthBufferTest)
		glDisable(GL_DEPTH_TEST);					// DEPTH Test OFF

	else
		glEnable(GL_DEPTH_TEST);						// Turn ON depth test

	if (pCurrentObject->disableDepthBufferWrite)
		glDisable(GL_DEPTH);						// DON'T Write to depth buffer

	else
		glEnable(GL_DEPTH);								// Write to depth buffer


	sModelDrawInfo drawInfo;
	if (pVAOManager->FindDrawInfoByModelName(pCurrentObject->meshName, drawInfo)) {
		glBindVertexArray(drawInfo.VAO_ID);
		glDrawElements(GL_TRIANGLES,
			drawInfo.numberOfIndices,
			GL_UNSIGNED_INT,
			0);
		glBindVertexArray(0);
	}

	return;
} // DrawObject;


// returns NULL (0) if we didn't find it.
cModelObject* pFindObjectByFriendlyName(std::string name) {
	// Do a linear search 
	for (unsigned int index = 0; index != g_vec_pGameObjects.size(); index++) {
		if (::g_vec_pGameObjects[index]->friendlyName == name) {
			// Found it!!
			return ::g_vec_pGameObjects[index];
		}
	}
	// Didn't find it
	return NULL;
}

// returns NULL (0) if we didn't find it.
cModelObject* pFindObjectByFriendlyNameMap(std::string name) {
	return ::mpModel[name];
}
