#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include <map>

#include "cMesh.hpp"
#include "iGameObject.hpp"

enum eShapeTypes {
	AABB,
	SPHERE,
	CUBE,
	PLANE,
	MESH,
	UNKNOWN
};


class cModelObject : public iGameObject {

public:
	/*Interface*/

	//unsigned int friendlyIDNumber;
	//std::string friendlyName;

	// bool isVisible;
	// bool isControl;

	////////////////////////////////////////////////

	/*Model Object*/

	/*Physics and Graphics*/
	std::string meshName;	// Used to draw this mesh

	/*Physics and Graphics*/
	glm::vec3  positionXYZ;
	glm::vec3  prevPositionXYZ;
	glm::vec3  rotationXYZ;
	float scale;

	/*Physics and Graphics*/
	glm::mat4 matWorld;

	/*Physics and Graphics*/
	bool isWireframe;
	glm::vec4 debugColour;
	bool disableDepthBufferTest;
	bool disableDepthBufferWrite;

	/*Graphics*/
	glm::vec4 objectColourRGBA;
	glm::vec4 diffuseColour;
	glm::vec4 specularColour;


	/*Physics*/
	glm::vec3 velocity;
	glm::vec3 accel;

	/*Physics*/
	float inverseMass;

	//bool bIsDynamic;

	/*Physics*/
	eShapeTypes physicsShapeType;

	cModelObject(std::vector<cModelObject*>& vModels);

	cModelObject(std::vector<cModelObject*>& vModels, std::string meshName, std::string fObjectName);

	/*Interface*/
	virtual unsigned int getUniqueID(void);
	virtual glm::vec3 getPosition(void);

protected:

	/*Interface*/
	//static unsigned int next_uniqueID;
	//unsigned int m_uniqueID;
};