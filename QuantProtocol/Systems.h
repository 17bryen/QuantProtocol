#pragma once

#include "Account.h"

using namespace std;
using namespace RApi;

const int LoginStatus_NotLoggedIn = 0;
const int LoginStatus_AwaitingResults = 1;
const int LoginStatus_Failed = 2;
const int LoginStatus_Complete = 3;

class Systems {
private:
	REngine* pEngine;
	void* pCallbacks;

	tsNCharcb user, pass;
	tsNCharcb rpCnnctPoint;
	tsNCharcb mdCnnctPoint, tsCnnctPoint, pnlCnnctPoint, ihCnnctPoint;

public:
	Account* acc;

	Systems(REngine* toEngine, void* toCallbacks);
	Systems();
	~Systems();

	bool getSysState();
	int getCurrentTime();

	int init();
	int login();
	int logout(bool close);
	int checkAgreements();

	bool setEngine(REngine* toEngine);
	bool setCallbacks(void* toCallbacks);
	bool setUser(char* name);
	bool setPass(char* word);

	bool setMdConnect(char* point);
	bool setTsConnect(char* point);
	bool setPnlConnect(char* point);
	bool setIhConnect(char* point);
	bool setRpConnect(char* point);

	/*	--------------------------------------------------------------------------	*/

	bool bRcvdUnacceptedAgreements = false;
	int iUnacceptedMandatoryAgreements = 0;

	int iRepLoginStatus = 0;
	int iMdLoginStatus = 0;
	int iTsLoginStatus = 0;
	int iIhLoginStatus = 0;
	int iPnlLoginStatus = 0;

	bool userManagement = true;
	bool stateManagement = true;
	bool orderManagement = false;
};

