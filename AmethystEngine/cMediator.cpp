#include "cMediator.hpp"
#include <iostream>

//enum _status { NOK, OK };

void cMediator::ConnectEndpointClients() {
	pCmdMngr = cCommandManager::GetCommandManager();
	pLgtMngr = cLightManager::GetLightManager();
	pMdlMngr = cModelManager::GetModelManager();
	pFilMngr = cFileManager::GetFileManager();

	iMediatorInterface* pMediatorInterface = this;
	pCmdMngr->setMediatorPointer(pMediatorInterface);
	pLgtMngr->setMediatorPointer(pMediatorInterface);
	pMdlMngr->setMediatorPointer(pMediatorInterface);
}

sNVData cMediator::RecieveMessage(sNVData message) {
	sNVData response;
	response.status = "OK";

	std::cout << message.cmd << std::endl;

	// wireframe on/off/toggle
	if (message.cmd == "wireframe") {
		sNVData data;
		data.cmd = "wireframe";
		if (message.state == "enable") { data.state = "enable"; }
		else if (message.state == "disable") { data.state = "disable"; }
		else if (message.state == "toggle") { data.state = "toggle"; }
		else { response.status == "NOK"; }
		if (message.target == "")
			data.pmo = g_vec_pGameObjects[vpGameObjectsCurrentPos];
		else
			data.pmo = mpModel[message.target];
		if (pMdlMngr->RecieveMessage(data).status == "NOK")
			response.status = "NOK";
	}

	else if (message.cmd == "wireframeall") {
		sNVData data;
		data.cmd = "wireframeall";
		if (message.state == "enable") { data.state = "enable"; }
		else if (message.state == "disable") { data.state = "disable"; }
		else if (message.state == "toggle") { data.state = "toggle"; }
		else { response.status == "NOK"; }
		data.vcmo = g_vec_pGameObjects;
		if (pMdlMngr->RecieveMessage(data).status == "NOK")
			response.status = "NOK";
	}

	// visibility on/off/toggle
	else if (message.cmd == "visible") {
		if (message.subsystem == "") {
			if (isModelSelect) { message.subsystem = "model"; }
			if (isLightSelect) { message.subsystem = "light"; }
			if (isComplexSelect) { message.subsystem = "complex"; }
		}

		if (message.subsystem == "model") {
			sNVData data;
			data.cmd = "visible";
			if (message.state == "enable") { data.state = "enable"; }
			else if (message.state == "disable") { data.state = "disable"; }
			else if (message.state == "toggle") { data.state = "toggle"; }
			else { response.status = "NOK"; }
			if (message.target == "") { data.pmo = g_vec_pGameObjects[vpGameObjectsCurrentPos]; }
			else { data.pmo = mpModel[message.target]; }
			if (response.status == "OK") {
				if (pMdlMngr->RecieveMessage(data).status == "NOK") { response.status == "NOK"; }
			}
		}
		else if (message.subsystem == "light") {
			sNVData data;
			data.cmd = "visible";
			if (message.state == "enable") { data.state = "enable"; }
			else if (message.state == "disable") { data.state = "disable"; }
			else if (message.state == "toggle") { data.state = "toggle"; }
			else { response.status = "NOK"; }
			if (message.target == "") { data.plo = g_vec_pLightObjects[vpLightObjectsCurrentPos]; }
			else { data.plo = mpLight[message.target]; }
			if (response.status == "OK") {
				if (pLgtMngr->RecieveMessage(data).status == "NOK") { response.status == "NOK"; }
			}
		}
		else { response.status = "NOK"; }
		return response;
	}

	// visibility all on/off/toggle
	else if (message.cmd == "visibleall") {
		if (message.subsystem == "") {
			if (isModelSelect) { message.subsystem = "model"; }
			if (isLightSelect) { message.subsystem = "light"; }
			if (isComplexSelect) { message.subsystem = "complex"; }
		}

		if (message.subsystem == "model") {
			sNVData data;
			data.cmd = "visibleall";
			if (message.state == "enable") { data.state = "enable"; }
			else if (message.state == "disable") { data.state = "disable"; }
			else if (message.state == "toggle") { data.state = "toggle"; }
			else { response.status == "NOK"; }
			data.vcmo = g_vec_pGameObjects;
			if (response.status == "OK") {
				if (pMdlMngr->RecieveMessage(data).status == "NOK") { response.status == "NOK"; }
			}
		}
		else if (message.subsystem == "light") {
			sNVData data;
			data.cmd = "visibleall";
			if (message.state == "enable") { data.state = "enable"; }
			else if (message.state == "disable") { data.state = "disable"; }
			else if (message.state == "toggle") { data.state = "toggle"; }
			else { response.status == "NOK"; }
			data.vclo = g_vec_pLightObjects;
			if (response.status == "OK") {
				if (pLgtMngr->RecieveMessage(data).status == "NOK") { response.status == "NOK"; }
			}
		}

		else { response.status = "NOK"; }
	}

	else if (message.cmd == "savefiles") {		
		if (message.target != "") {
			if (!pFilMngr->SaveData(message._models_filename, message._lights_filename, message._complex_filename))
				response.status = "NOK";
		}
		
		else {
			if (!pFilMngr->SaveData())
				response.status = "NOK";
		}
	}

	else if (message.cmd == "control") {
		if (message.subsystem == "models") {
			isModelSelect = true;
			isLightSelect = false;
			isComplexSelect = false;
		}

		else if (message.subsystem == "lights") {
			isModelSelect = false;
			isLightSelect = true;
			isComplexSelect = false;
		}

		else if (message.subsystem == "complex") {
			isModelSelect = false;
			isLightSelect = false;
			isComplexSelect = true;
		}

		else
			response.status = "NOK";
	}
	
	else if (message.cmd == "create") {
		sNVData data;
		data.cmd = "create";
		if (message.subsystem == "") {
			if (isModelSelect) { message.subsystem = "model"; }
			if (isLightSelect) { message.subsystem = "light"; }
			if (isComplexSelect) { message.subsystem = "complex"; }
		}
		if (message.subsystem == "model") {
			data.subsystem = "model";
			data.mesh = message.mesh;
			data.name = message.name;
			if(response.status == "OK")
				if (pMdlMngr->RecieveMessage(data).status == "NOK") { response.status == "NOK"; }
		}
		else if (message.subsystem == "light") {
			data.subsystem = "light";
			data.name = message.name;
			if (response.status == "OK")
				if (pLgtMngr->RecieveMessage(data).status == "NOK") { response.status == "NOK"; }
		}
		else if (message.subsystem == "complex") { }
		else { response.status = "NOK"; }
	}


	else
		response.status = "NOK";

	return response;
}
