#include "Systems.h"

using namespace std;
using namespace RApi;


Systems::Systems(REngine* toEngine, void* toCallbacks) {
	pEngine = toEngine;
	pCallbacks = toCallbacks;

	/*	---------------- User, Pass, and Ptrs are set to NULL ----------------	*/

	acc = new Account();

	user.pData = NULL;
	user.iDataLen = 0;
	pass.pData = NULL;
	pass.iDataLen = 0;

	/*	--------------- Provide Deault Values to Connect Points --------------	*/

	mdCnnctPoint.pData = (char*)"login_agent_tp_r01c"; //Use login_agent_tp_agg_r01c for aggregated data
	mdCnnctPoint.iDataLen = (int)strlen(mdCnnctPoint.pData);

	tsCnnctPoint.pData = (char*)"login_agent_prodc";
	tsCnnctPoint.iDataLen = (int)strlen(tsCnnctPoint.pData);

	pnlCnnctPoint.pData = (char*)"login_agent_pnl_sslc";
	pnlCnnctPoint.iDataLen = (int)strlen(pnlCnnctPoint.pData);

	ihCnnctPoint.pData = (char*)"login_agent_historyc";
	ihCnnctPoint.iDataLen = (int)strlen(ihCnnctPoint.pData);

	rpCnnctPoint.pData = (char*)"login_agent_repositoryc";
	rpCnnctPoint.iDataLen = (int)strlen(rpCnnctPoint.pData);

}
Systems::Systems() {
	/*	---------------- User, Pass, and Ptrs are set to NULL ----------------	*/

	pEngine = nullptr;
	pCallbacks = nullptr;

	acc = new Account();

	user.pData = NULL;
	user.iDataLen = 0;
	pass.pData = NULL;
	pass.iDataLen = 0;

	/*	--------------- Provide Deault Values to Connect Points --------------	*/

	mdCnnctPoint.pData = (char*)"login_agent_tp_r01c"; //Use login_agent_tp_agg_r01c for aggregated data
	mdCnnctPoint.iDataLen = (int)strlen(mdCnnctPoint.pData);

	tsCnnctPoint.pData = (char*)"login_agent_prodc";
	tsCnnctPoint.iDataLen = (int)strlen(tsCnnctPoint.pData);

	pnlCnnctPoint.pData = (char*)"login_agent_pnl_sslc";
	pnlCnnctPoint.iDataLen = (int)strlen(pnlCnnctPoint.pData);

	ihCnnctPoint.pData = (char*)"login_agent_historyc";
	ihCnnctPoint.iDataLen = (int)strlen(ihCnnctPoint.pData);

	rpCnnctPoint.pData = (char*)"login_agent_repositoryc";
	rpCnnctPoint.iDataLen = (int)strlen(rpCnnctPoint.pData);

}
Systems::~Systems() {

}

/*	==========================================================================	*/

bool Systems::getSysState() {
	if (iMdLoginStatus != LoginStatus_Complete ||
		iTsLoginStatus != LoginStatus_Complete ||
		iPnlLoginStatus != LoginStatus_Complete ||
		iIhLoginStatus != LoginStatus_Complete ||
		!userManagement)
		return false;

	return true;
}

/*	==========================================================================	*/

int Systems::init() {
	acc->setEngine(pEngine);
	return 0;
}
/* Logs the Quant object into the connect points using stored values. Be sure
*	to call setUser() and setPass() prior to this point.
*/
int Systems::login() {
	int iCode;
	LoginParams oLoginParams;

	oLoginParams.pCallbacks = (RCallbacks*)pCallbacks;

	oLoginParams.sUser = user;
	oLoginParams.sPassword = pass;
	oLoginParams.sMdUser = user;
	oLoginParams.sMdPassword = pass;
	oLoginParams.sIhUser = user;
	oLoginParams.sIhPassword = pass;

	oLoginParams.sMdCnnctPt = mdCnnctPoint;
	oLoginParams.sTsCnnctPt = tsCnnctPoint;
	oLoginParams.sPnlCnnctPt = pnlCnnctPoint;
	oLoginParams.sIhCnnctPt = ihCnnctPoint;

	if (!pEngine->login(&oLoginParams, &iCode)) {
		cout << endl << "REngine::login() error : " << iCode << endl;
		return 1;
	}

	while ((iMdLoginStatus != LoginStatus_Complete && iMdLoginStatus != LoginStatus_Failed) ||
		(iTsLoginStatus != LoginStatus_Complete && iTsLoginStatus != LoginStatus_Failed) ||
		(iPnlLoginStatus != LoginStatus_Complete && iPnlLoginStatus != LoginStatus_Failed) ||
		(iIhLoginStatus != LoginStatus_Complete && iIhLoginStatus != LoginStatus_Failed))
		Sleep(1000);

	if (iMdLoginStatus == LoginStatus_Failed ||
		iTsLoginStatus == LoginStatus_Failed ||
		iPnlLoginStatus == LoginStatus_Failed ||
		iIhLoginStatus == LoginStatus_Failed) {
		cout << endl << endl;
		cout << "REngine::Login() error : Make sure your username and password are correct and match the system you are trying to log onto." << endl;

		return 1;
	}

	return 0;
}
int Systems::logout(bool close) {
	int iCode;
	if (!pEngine->logout(&iCode)) {
		cout << endl << endl;
		cout << "REngine::Logout() error : Honestly is this really bad?" << endl;

		return 1;
	}

	if (close)
		delete this;

	return 0;
}
int Systems::checkAgreements() {
	int iCode;

	// Login to Repo
	if (!pEngine->loginRepository(&user, &pass, &rpCnnctPoint, (RCallbacks*)pCallbacks, &iCode)) {
		cout << "REngine::loginRepository() error : " << iCode << endl;

		return 1;
	}

	// Await Repo Response
	while (iRepLoginStatus != LoginStatus_Complete && iRepLoginStatus != LoginStatus_Failed)
		Sleep(1000);

	if (iRepLoginStatus == LoginStatus_Failed) {
		cout << endl << endl;
		cout << "REngine::LoginRepository() error : Make sure your username and password are correct and match the system you are trying to log onto." << endl;

		return 1;
	}

	// Req List of Unaccepted Agreements
	if (!pEngine->listAgreements(false, NULL, &iCode)) {
		cout << "REngine::listAgreements() error : " << iCode << endl;

		pEngine->logoutRepository(&iCode);

		return 1;
	}

	// Await list
	while (!bRcvdUnacceptedAgreements)
		Sleep(1000);

	// Check unaccepted agreement amount
	if (iUnacceptedMandatoryAgreements > 0) {
		cout << endl << endl;
		cout << "Unaccepted agreements are associated with this account. Please login to R | Trader and sign agreements." << endl;

		pEngine->logoutRepository(&iCode);

		return 1;
	}

	// No unsigned agreements - logout from repository
	pEngine->logoutRepository(&iCode);
	return 0;
}

/*	==========================================================================	*/

bool Systems::setEngine(REngine* toEngine) {
	pEngine = toEngine;
	return true;
}
bool Systems::setCallbacks(void* toCallbacks) {
	pCallbacks = toCallbacks;
	return true;
}
bool Systems::setUser(char* name) {
	user.pData = name;
	user.iDataLen = (int)strlen(user.pData);
	return true;
}
bool Systems::setPass(char* word) {
	pass.pData = word;
	pass.iDataLen = (int)strlen(pass.pData);
	return true;
}

bool Systems::setMdConnect(char* point) {
	if (point != NULL) {
		mdCnnctPoint.pData = (char*)point;
		mdCnnctPoint.iDataLen = (int)strlen(mdCnnctPoint.pData);
		return true;
	}
	else {
		mdCnnctPoint.pData = (char*)"login_agent_tp_r01c"; //Use login_agent_tp_agg_r01c for aggregated data
		mdCnnctPoint.iDataLen = (int)strlen(mdCnnctPoint.pData);
		return false;
	}
}
bool Systems::setTsConnect(char* point) {
	if (point != NULL) {
		tsCnnctPoint.pData = (char*)point;
		tsCnnctPoint.iDataLen = (int)strlen(tsCnnctPoint.pData);
		return true;
	}
	else {
		tsCnnctPoint.pData = (char*)"login_agent_prodc";
		tsCnnctPoint.iDataLen = (int)strlen(tsCnnctPoint.pData);
		return false;
	}
}
bool Systems::setPnlConnect(char* point) {
	if (point != NULL) {
		pnlCnnctPoint.pData = (char*)point;
		pnlCnnctPoint.iDataLen = (int)strlen(pnlCnnctPoint.pData);
		return true;
	}
	else {
		pnlCnnctPoint.pData = (char*)"login_agent_pnl_sslc";
		pnlCnnctPoint.iDataLen = (int)strlen(pnlCnnctPoint.pData);
		return false;
	}
}
bool Systems::setIhConnect(char* point) {
	if (point != NULL) {
		ihCnnctPoint.pData = (char*)point;
		ihCnnctPoint.iDataLen = (int)strlen(ihCnnctPoint.pData);
		return true;
	}
	else {
		ihCnnctPoint.pData = (char*)"login_agent_historyc";
		ihCnnctPoint.iDataLen = (int)strlen(ihCnnctPoint.pData);
		return false;
	}
}
bool Systems::setRpConnect(char* point) {
	if (point != NULL) {
		rpCnnctPoint.pData = (char*)point;
		rpCnnctPoint.iDataLen = (int)strlen(rpCnnctPoint.pData);
		return true;
	}
	else {
		rpCnnctPoint.pData = (char*)"login_agent_repositoryc";
		rpCnnctPoint.iDataLen = (int)strlen(rpCnnctPoint.pData);
		return false;
	}
}
