#pragma once
#include "sNVData.hpp"

class iMediatorInterface {
public:
	virtual ~iMediatorInterface() { };
	virtual sNVData RecieveMessage(sNVData message) = 0;
};
