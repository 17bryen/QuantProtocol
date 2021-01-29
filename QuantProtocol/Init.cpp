#include "Init.h"
#include "Main.h"

using namespace std;
using namespace RApi;

int initAdmCallbacks() {
	int iCode;
	/*	=============================== Init AdmCallbacks ===========================	*/
	cout << "initAdmCallbacks() has been called...";
	try {
		g->pAdmCallbacks = new ImplAdmCallbacks();
		cout << "Created AdmCallbacks object... ";
	}
	catch (OmneException& oEx) {
		iCode = oEx.getErrorCode();
		cout << "ImplAdmCallbacks::ImplAdmCallbacks() error : " << iCode << endl;
		return 1;
	}
	return 0;
}

int initCallbacks() {
	int iCode;

	/*	=================== Init Callbacks - PreReq to Logging in ===================	*/

	try {
		g->pCallbacks = new ImplCallbacks();
		cout << "Created callbacks object." << endl;
	}
	catch (OmneException& oEx) {
		delete g->pEngine;
		delete g->pAdmCallbacks;

		iCode = oEx.getErrorCode();
		cout << "MyCallbacks::MyCallbacks() error : " << iCode << endl;
		return 1;
	}

	return 0;
}

int initREngine(REngineParams &oParams) {
	int iCode;

	/*	================================== Init REngine =============================	*/

	try {
		g->pEngine = new REngine(&oParams);
		cout << "Created REngine object... ";
	}
	catch (OmneException& oEx) {
		delete g;

		iCode = oEx.getErrorCode();
		cout << "REnging::REngine() error : " << iCode << endl;
		return 1;
	}

	return 0;
}