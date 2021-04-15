#include "cLightObject.hpp"

cLightObject::cLightObject(std::vector<cLightObject*> &vLights) {

	/*Interface*/
	this->friendlyIDNumber = 0;
	this->friendlyName = "";

	this->isVisible = true;
	this->isControl = false;

	this->m_uniqueID = next_uniqueID;

	// Then increment the static variable
	++cLightObject::next_uniqueID;

	////////////////////////////////////////////////

	/*Light Object*/
	position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	atten = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	direction = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	param1 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	param2 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	parentObject = NULL;

	// add the "factory" settings light to the array of lights
	vLights.push_back(this);
};

cLightObject::cLightObject(std::vector<cLightObject*>& vLights, std::string fObjectName) {

	/*Interface*/
	this->friendlyIDNumber = 0;
	this->friendlyName = fObjectName;

	this->isVisible = true;
	this->isControl = false;

	this->m_uniqueID = next_uniqueID;

	// Then increment the static variable
	++cLightObject::next_uniqueID;

	////////////////////////////////////////////////

	/*Light Object*/
	position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	atten = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	direction = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	param1 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	param2 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	parentObject = NULL;

	// add the "factory" settings light to the array of lights
	vLights.push_back(this);
};

unsigned int cLightObject::getUniqueID(void) { return this->m_uniqueID; }

glm::vec3 cLightObject::getPosition(void)
{
	return glm::vec3(this->position.x, this->position.y, this->position.z);
}
