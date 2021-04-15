#include <iostream>
#include "cModelManager.hpp"


#pragma region SINGLETON
cModelManager cModelManager::stonModelMngr;
cModelManager* cModelManager::GetModelManager() { return &(cModelManager::stonModelMngr); }
cModelManager::cModelManager() {
	std::cout << "Model Manager Created" << std::endl;
	pMediator = nullptr;
}
#pragma endregion



#pragma region MEDIATOR_COMMUNICATION
void cModelManager::setMediatorPointer(iMediatorInterface* pMediator) {
	this->pMediator = pMediator;
	return;
}



sNVData cModelManager::RecieveMessage(sNVData message) {
	sNVData response;
	response.status = "OK";
	
	if (message.cmd == "wireframe") {
		if (message.state == "enable") {
			if (!EnableWireframe(message.pmo))
				response.status = "NOK";
		}
		else if (message.state == "disable") {
			if (!DisableWireframe(message.pmo))
				response.status = "NOK";
		}
		else if (message.state == "toggle") {
			if (!ToggleWireframe(message.pmo))
				response.status = "NOK";
		}
		else
			response.status = "NOK";
	}

	else if (message.cmd == "wireframeall") {
		if (message.state == "enable") {
			if (!EnableWireframe(message.vcmo))
				response.status = "NOK";
		}
		else if (message.state == "disable") {
			if (!DisableWireframe(message.vcmo))
				response.status = "NOK";
		}
		else if (message.state == "toggle") {
			if (!ToggleWireframe(message.vcmo))
				response.status = "NOK";
		}
		else
			response.status = "NOK";
	}

	else if (message.cmd == "visible") {
		if (message.state == "enable") {
			if (!EnableVisible(message.pmo))
				response.status = "NOK";
		}
		else if (message.state == "disable") {
			if (!DisableVisible(message.pmo))
				response.status = "NOK";
		}
		else if (message.state == "toggle") {
			if (!ToggleVisible(message.pmo))
				response.status = "NOK";
		}
		else
			response.status = "NOK";
	}

	else if (message.cmd == "visibleall") {
		if (message.state == "enable") {
			if (!EnableVisible(message.vcmo))
				response.status = "NOK";
		}
		else if (message.state == "disable") {
			if (!DisableVisible(message.vcmo))
				response.status = "NOK";
		}
		else if (message.state == "toggle") {
			if (!ToggleVisible(message.vcmo))
				response.status = "NOK";
		}
		else
			response.status = "NOK";
	}

	else if (message.cmd == "create") {
		if (_fact_game_obj.CreateMediatedGameObject(message.subsystem, message.mesh, message.name) == NULL)
			response.status = "NOK";
	}

	else
		response.status = "NOK";
	return response;
}
#pragma endregion



#pragma region SINGULAR CREATE OBJECT DEFAULT/INITIALIZED [ NYI ]

#pragma endregion



#pragma region SINGULAR WIREFRAME ON/OFF/TOGGLE
bool cModelManager::EnableWireframe(cModelObject*& pModel) {
	if (pModel != nullptr) {
		pModel->isWireframe = true;
		if (pModel->isWireframe == false) { return false; }
	}
	else
		return false;
	return true;
}

bool cModelManager::DisableWireframe(cModelObject*& pModel) {
	if (pModel != nullptr) {
		pModel->isWireframe = false;
		if (pModel->isWireframe == true) { return false; }
	}
	else
		return false;
	return true;
}

bool cModelManager::ToggleWireframe(cModelObject*& pModel) { return (pModel->isWireframe == true ? DisableWireframe(pModel) : EnableWireframe(pModel)); }
#pragma endregion



#pragma region ALL WIREFRAME ON/OFF/TOGGLE
bool cModelManager::EnableWireframe(std::vector<cModelObject*> &pModels) {
	for (cModelObject* cmo : pModels) {
		cmo->isWireframe = true;
		if (cmo->isWireframe == false) { return false; }
	}
	return true;
}

bool cModelManager::DisableWireframe(std::vector<cModelObject*>& pModels) {
	for (cModelObject* cmo : pModels) {
		cmo->isWireframe = false;
		if (cmo->isWireframe == true) { return false; }
	}
	return true;
}

bool cModelManager::ToggleWireframe(std::vector<cModelObject*>& pModels) {
	for (cModelObject* cmo : pModels) {
		if (!ToggleWireframe(cmo))
			return false;
	}
	return true;
}
#pragma endregion



#pragma region SINGULAR VISIBILITY ON/OFF/TOGGLE
bool cModelManager::EnableVisible(cModelObject*& pModel) {
	pModel->isVisible = true;
	if (pModel->isVisible == false) { return false; }
	return true;
}

bool cModelManager::DisableVisible(cModelObject*& pModel) {
	pModel->isVisible = false;
	if (pModel->isVisible == true) { return false; }
	return true;
}

bool cModelManager::ToggleVisible(cModelObject*& pModel) { return (pModel->isVisible == true ? DisableVisible(pModel) : EnableVisible(pModel)); }
#pragma endregion



#pragma region ALL VISIBILITY ON/OFF/TOGGLE
bool cModelManager::EnableVisible(std::vector<cModelObject*>& pModels) {
	for (cModelObject* cmo : pModels) {
		cmo->isVisible = true;
		if (cmo->isVisible == false) { return false; }
	}
	return true;
}

bool cModelManager::DisableVisible(std::vector<cModelObject*>& pModels) {
	for (cModelObject* cmo : pModels) {
		cmo->isVisible = false;
		if (cmo->isVisible == true) { return false; }
	}
	return true;
}

bool cModelManager::ToggleVisible(std::vector<cModelObject*>& pModels) {
	for (cModelObject* cmo : pModels) {
		if (!ToggleVisible(cmo))
			return false;
	}
	return true;
}
#pragma endregion
