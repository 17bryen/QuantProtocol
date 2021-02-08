#pragma once
#include "Globals.h"
#include "ImplRApi.h"
#include <vector>

const int LoginStatus_NotLoggedIn = 0;
const int LoginStatus_AwaitingResults = 1;
const int LoginStatus_Failed = 2;
const int LoginStatus_Complete = 3;

class Quant {
	/*	======================== Declare Member Variables ========================	*/
public:
	REngine* pEngine;
	ImplAdmCallbacks* pAdmCallbacks;
	ImplCallbacks* pCallbacks;

	globals* callbackResponses;
	vector<Contract> watchList;

	/*	--------------------------------------------------------------------------	*/
private:
	tsNCharcb user, pass;
	tsNCharcb rpCnnctPoint;
	tsNCharcb mdCnnctPoint, tsCnnctPoint, pnlCnnctPoint, ihCnnctPoint;


	/*	======================== Declare Member Functions ========================	*/
public:
	Quant(char* md, char* ts, char* pnl, char* ih, char* rp);
	Quant();
	~Quant();

	/*	--------------------------------------------------------------------------	*/

	int init(REngineParams &oParams);
	int login();
	int logout(bool close);

	int checkAgreements();

	/*	--------------------------------------------------------------------------	*/



	/*	--------------------------------------------------------------------------	*/

	bool setUser(char* name);
	bool setPass(char* word);

	bool setMdConnect(char* point);
	bool setTsConnect(char* point);
	bool setPnlConnect(char* point);
	bool setIhConnect(char* point);
	bool setRpConnect(char* point);
};


