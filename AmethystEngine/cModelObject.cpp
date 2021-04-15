#include "cModelObject.hpp"

cModelObject::cModelObject(std::vector<cModelObject*>& vModels) {

	/*Interface*/
	this->friendlyIDNumber = 0;
	this->friendlyName = "";

	this->isVisible = true;
	this->isControl = false;

	this->m_uniqueID = next_uniqueID;

	++cModelObject::next_uniqueID;

	////////////////////////////////////////////////

	/*Model Object*/

	this->meshName = "";

	this->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	this->prevPositionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scale = 1.0f;

	this->matWorld = glm::mat4(1.0f);

	/*this->HACK_speed = 0.0f;
	this->HACK_AngleAroundYAxis = 0.0f;*/

	this->isWireframe = false;
	this->debugColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->disableDepthBufferTest = false;
	this->disableDepthBufferWrite = false;

	this->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->diffuseColour = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	this->specularColour = glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f);

	this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	this->accel = glm::vec3(0.0f, 0.0f, 0.0f);

	this->inverseMass = 0.0f;

	this->physicsShapeType = UNKNOWN;

	vModels.push_back(this);
}


cModelObject::cModelObject(std::vector<cModelObject*>& vModels, std::string meshName, std::string fObjectName) {

	/*Interface*/
	this->friendlyIDNumber = 0;
	this->friendlyName = fObjectName;

	this->isVisible = true;
	this->isControl = false;

	this->m_uniqueID = next_uniqueID;

	++cModelObject::next_uniqueID;

	////////////////////////////////////////////////

	/*Model Object*/

	this->meshName = meshName;

	this->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scale = 1.0f;

	this->matWorld = glm::mat4(1.0f);

	//this->HACK_speed = 0.0f;
	//this->HACK_AngleAroundYAxis = 0.0f;

	this->isWireframe = false;
	this->debugColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->disableDepthBufferTest = false;
	this->disableDepthBufferWrite = false;

	this->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->diffuseColour = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	this->specularColour = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

	this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	this->accel = glm::vec3(0.0f, 0.0f, 0.0f);

	this->inverseMass = 0.0f;

	this->physicsShapeType = UNKNOWN;

	vModels.push_back(this);
}

unsigned int cModelObject::getUniqueID(void) { return this->m_uniqueID; }

glm::vec3 cModelObject::getPosition(void)
{
	return this->positionXYZ;
}

unsigned int iGameObject::next_uniqueID = 1000;
