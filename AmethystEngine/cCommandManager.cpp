#include <iostream>
#include "cCommandManager.hpp"



#pragma region SINGLETION
cCommandManager cCommandManager::stonModelMngr;
cCommandManager* cCommandManager::GetCommandManager() { return &(cCommandManager::stonModelMngr); }
cCommandManager::cCommandManager() { std::cout << "Command Manager Created" << std::endl; pMediator = nullptr; }
#pragma endregion



#pragma region MEDIATOR_COMMUNICATIONS
void cCommandManager::setMediatorPointer(iMediatorInterface* pMediator) {
	this->pMediator = pMediator;
	return;
}



void cCommandManager::ConvertToLower(std::string cmdstr) {
	std::string s;
	if (!cmdstr.empty()) {
		if (cmdstr.front() == '`')
			cmdstr = cmdstr.substr(1, cmdstr.back());
		for (char c : cmdstr)
			s += std::tolower(c);
		cmdstr = s;
	}
}



void cCommandManager::IssueCommand(std::string cmd) {
	sNVData data;

	std::string temp;

	// parse cmd for data population
	std::vector<std::string> vdata;
	
	bool targetEntry = false;
	for (char c : cmd) {
		if (c == '.') {
			vdata.push_back(temp);
			temp = "";
		}
		else if (c == '"') {
			if (vdata.size() == 0)
				vdata.push_back("");
			if(targetEntry == false)
				targetEntry = true;
			else {
				if (vdata.size() == 0)
					vdata.push_back("");
				targetEntry = false;
				vdata.push_back(temp);
				temp = "";
			}
		}
		else if (c == ' ') {
			if (vdata.size() == 0)
				vdata.push_back("");
			if (!temp.empty())
				vdata.push_back(temp);
			temp = "";
		}
		else {
			temp += c;
		}
	}

	if (!temp.empty()) {
		vdata.push_back(temp);
		temp = "";
	}

	// detection for specific commands
	// i.e. save, control, etc.

	std::cout << vdata.size() << std::endl;

	unsigned i = 0;
	for (std::string s : vdata) {
		std::cout << "STRING OF " << i << " VALUE [ " << s << " ]" << std::endl;
		++i;
	}
	i = 0;


	// SPECIAL SINGLE TYPE COMMANDS
	if (vdata.size() == 1) {
		ConvertToLower(vdata[0]);
		data.cmd = vdata[0];
	}

	else if (vdata[1] == "savefiles") {
		ConvertToLower(vdata[1]);
		data.cmd = vdata[1];
		data.target = "M";
		data._models_filename = vdata[2];
		data._lights_filename = vdata[3];
		data._complex_filename = vdata[4];
	}

	else if (vdata[1] == "control") {
		ConvertToLower(vdata[0]);
		ConvertToLower(vdata[1]);
		data.subsystem = vdata[0];
		data.cmd = vdata[1];
	}

	else {
		if (vdata[1] == "create") {
			if (vdata[0] == "model") {
				ConvertToLower(vdata[0]);
				ConvertToLower(vdata[1]);

				data.subsystem = vdata[0];
				data.cmd = vdata[1];
				data.mesh = vdata[2];
				data.name = vdata[3];
			}
			else if (vdata[0] == "light") {
				ConvertToLower(vdata[0]);
				ConvertToLower(vdata[1]);

				data.subsystem = vdata[0];
				data.cmd = vdata[1];
				data.name = vdata[2];
			}
		}
		else if (vdata.size() == 3) {
			ConvertToLower(vdata[0]);
			ConvertToLower(vdata[1]);
			ConvertToLower(vdata[2]);

			if (vdata[0] != "")
				data.subsystem = vdata[0];
			data.cmd = vdata[1];
			data.state = vdata[2];
		}
		else if (vdata[0] != "") {
			if (vdata[0] != "") {
				ConvertToLower(vdata[0]);

				data.subsystem = vdata[0];
			}

			ConvertToLower(vdata[1]);
			data.cmd = vdata[1];
			data.target = vdata[2];

			if (vdata.size() == 4) {
				ConvertToLower(vdata[3]);
				data.state = vdata[3];
			}
		}
		else {
			ConvertToLower(vdata[0]);
			ConvertToLower(vdata[1]);
			ConvertToLower(vdata[3]);

			data.cmd = vdata[1];
			data.state = vdata[3];
			data.target = vdata[2];
			data.subsystem = vdata[0];
		}
	}
	this->RecieveMessage(data);
}




sNVData cCommandManager::RecieveMessage(sNVData message) {
	sNVData response;
	response.status = "OK";

	if(message.status == "OK") { }
	else if(message.status == "NOK") { }

	if(pMediator->RecieveMessage(message).status == "NOK")
		std::cout << "UNKNOWN COMMAND" << std::endl;
	else
		std::cout << "COMMAND ACCEPTED" << std::endl;

	return response;
}
#pragma endregion
