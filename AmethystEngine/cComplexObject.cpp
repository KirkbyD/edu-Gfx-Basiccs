#include "cComplexObject.hpp"
#include "Globals.hpp"

cComplexObject::cComplexObject(std::vector<cComplexObject*> &vObjects) {
	// ************************************
	// Interface
	this->friendlyIDNumber = 0;
	this->friendlyName = "";

	this->isVisible = true;
	this->isControl = false;

	this->m_uniqueID = next_uniqueID;

	this->parentObject = NULL;
	// ************************************

	this->scale = 1.0f;
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	this->inverseMass = 0.0f;
	this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	this->acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

	vObjects.push_back(this);
}

cComplexObject::~cComplexObject() {
	for (cLightObject* light : lightVector)	{
		light->parentObject = NULL;
	}

	for (cModelObject* model : modelVector)	{
		model->parentObject = NULL;
	}
}

float cComplexObject::GetScale()
{
	return this->scale;
}

glm::vec3 cComplexObject::GetPosition()
{
	return this->position;
}

glm::vec3 cComplexObject::GetPreviousPosition()
{
	return this->prevPosition;
}

glm::vec3 cComplexObject::GetRotation()
{
	return this->rotation;
}

float cComplexObject::GetInverseMass()
{
	return this->inverseMass;
}

glm::vec3 cComplexObject::GetVelocity()
{
	return this->velocity;
}

glm::vec3 cComplexObject::GetAcceleration()
{
	return this->acceleration;
}

void cComplexObject::SetScale(float val)
{
	this->scale = val;
	Resize();
}

void cComplexObject::SetPosition(glm::vec3 posVec)
{
	glm::vec3 movementVector = posVec - this->position; 
	Move(movementVector);
}

void cComplexObject::SetPreviousPosition(glm::vec3 posVec)
{
	this->prevPosition = posVec;
}

void cComplexObject::SetRotation(glm::vec3 rotVec)
{
	this->rotation = rotVec;
	Rotate(this->rotation);
}

void cComplexObject::SetInverseMass(float val)
{
	this->inverseMass = val;
}

void cComplexObject::SetVelocity(glm::vec3 velocity)
{
	this->velocity = velocity;
	for (cModelObject* model : this->modelVector) {
		model->velocity = this->velocity;
	}
}

void cComplexObject::SetAcceleration(glm::vec3 accel) 
{
	this->acceleration = accel;
	for (cModelObject* model : this->modelVector) {
		model->accel = this->acceleration;
	}
}

void cComplexObject::AddLight(std::string lightName) {
	if (mpLight.find(lightName) != mpLight.end()) {
		this->lightVector.push_back(mpLight[lightName]);
		this->lightVector.back()->parentObject = this;
		this->lightVector.back()->isControl = false;

		if (this->modelVector.size() == 0 && this->lightVector.size() == 1)
			this->position = lightVector[0]->position;
	}
}

void cComplexObject::AddModel(std::string friendlyName) {

	this->modelVector.push_back(pFindObjectByFriendlyName(friendlyName));
	this->modelVector.back()->parentObject = this;
	this->modelVector.back()->isControl = false;
	this->modelVector.back()->accel = this->acceleration;
	this->modelVector.back()->velocity = this->velocity;

	if (this->modelVector.size() == 1)
		this->position = modelVector[0]->positionXYZ;
}

void cComplexObject::Move(glm::vec3 movement) {
	position += movement;

	for (cModelObject* model : modelVector) {
		model->positionXYZ += movement;
	}
	for (cLightObject* light : lightVector) {
		light->position.x += movement.x;
		light->position.y += movement.y;
		light->position.z += movement.z;
	}
}

void cComplexObject::Resize() {
	for (cModelObject* model : modelVector) {
		model->positionXYZ -= position;
		model->positionXYZ *= this->scale;
		model->scale *= this->scale;
		model->positionXYZ += position;
	}

	for (cLightObject* light : lightVector) {
		light->position.x -= position.x;
		light->position.y -= position.y;
		light->position.z -= position.z;
		light->position *= this->scale;
		light->position.x += position.x;
		light->position.y += position.y;
		light->position.z += position.z;
	}
}

//need some fun trig or radius math I think to make this work
void cComplexObject::Rotate(glm::vec3 rotations) {
	if (isControl) {
		if (!modelVector.empty())
			position = modelVector[0]->positionXYZ;
		else if (!lightVector.empty())
			position = lightVector[0]->position;
	}
}

unsigned int cComplexObject::getUniqueID(void) { return this->m_uniqueID; }

glm::vec3 cComplexObject::getPosition(void)
{
	return this->position;
}

std::vector<cModelObject*> cComplexObject::GetModels() { return this->modelVector; }
std::vector<cLightObject*> cComplexObject::GetLights() { return this->lightVector; }
