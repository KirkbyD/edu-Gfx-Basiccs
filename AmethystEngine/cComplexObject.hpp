#pragma once
#include "cModelObject.hpp"
#include "cLightObject.hpp"
#include "iGameObject.hpp"
#include <vector>
#include <string>

class cComplexObject : public iGameObject {
private:
	// Graphics
	float scale;
	glm::vec3 position;
	glm::vec3 prevPosition;
	glm::vec3 rotation;

	// Physics
	float inverseMass;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	// Members
	std::vector<cModelObject*> modelVector;
	std::vector<cLightObject*> lightVector;

public:
	cComplexObject(std::vector<cComplexObject*> &vObjects);
	~cComplexObject();

	float GetScale();
	glm::vec3 GetPosition();
	glm::vec3 GetPreviousPosition();
	glm::vec3 GetRotation();
	float GetInverseMass();
	glm::vec3 GetVelocity();
	glm::vec3 GetAcceleration();
	std::vector<cModelObject*> GetModels();
	std::vector<cLightObject*> GetLights();

	void SetScale(float val);
	void SetPosition(glm::vec3 posVec);
	void SetPreviousPosition(glm::vec3 posVec);
	void SetRotation(glm::vec3 rotVec);
	void SetInverseMass(float val);
	void SetVelocity(glm::vec3 velocity);
	void SetAcceleration(glm::vec3 accel);

	void AddLight(std::string);
	void AddModel(std::string);		// has physics

	void Move(glm::vec3 movement);
	void Resize();
	void Rotate(glm::vec3 rotations);

	/*Interface*/
	virtual unsigned int getUniqueID(void);
	virtual glm::vec3 getPosition(void);
};