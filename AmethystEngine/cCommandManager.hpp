#pragma once
#include "iMediatorInterface.hpp"

class cCommandManager : iMediatorInterface {
	cCommandManager();
	static cCommandManager stonModelMngr;

	iMediatorInterface* pMediator;

	void ConvertToLower(std::string s);


public:

	static cCommandManager* GetCommandManager();


	void IssueCommand(std::string cmd);
	void setMediatorPointer(iMediatorInterface* pMediator);

	virtual sNVData RecieveMessage(sNVData message);

};
