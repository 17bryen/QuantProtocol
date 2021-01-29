#include "Logins.h"
#include "Main.h"


char* user = (char*)"1gDKI8QoY7";
char* pass = (char*)"";

int checkAgreements(tsNCharcb connectPoint) {
	int iCode;

	/*	=========================== Check Repo For Agreements ========================	*/
	iCode = loginRepo(connectPoint);
	if (iCode != 0) 
		return iCode;
	

	// Req List of Unaccepted Agreements
	if (!g->pEngine->listAgreements(false, NULL, &iCode)) {
		cout << "REngine::listAgreements() error : " << iCode << endl;

		logoutRepo();
		delete g;

		return 1;
	}

	// Await list
	while (!g->bRcvdUnacceptedAgreements)
		Sleep(1000);

	// Check unaccepted agreement amount
	if (g->iUnacceptedMandatoryAgreements > 0) {
		cout << endl << endl;
		cout << "Unaccepted agreements are associated with this account. Please login to R | Trader and sign agreements." << endl;

		logoutRepo();
		delete g;

		return 1;
	}

	// No unsigned agreements - logout from repository
	iCode = logoutRepo();
	if (iCode != 0)
		return iCode;

	return 0;

}

int loginRepo(tsNCharcb connectPoint) {
	tsNCharcb sUser, sPassword;
	int iCode;

	sUser.pData = user;
	sUser.iDataLen = (int)strlen(sUser.pData);

	sPassword.pData = pass;
	sPassword.iDataLen = (int)strlen(sPassword.pData);

	// Login to Repo
	if (!g->pEngine->loginRepository(&sUser, &sPassword, &connectPoint, g->pCallbacks, &iCode)) {
		cout << "REngine::loginRepository() error : " << iCode << endl;

		delete g;

		return 1;
	}

	// Await Repo Response
	while (g->iRepLoginStatus != LoginStatus_Complete && g->iRepLoginStatus != LoginStatus_Failed)
		Sleep(1000);

	if (g->iRepLoginStatus == LoginStatus_Failed) {
		cout << endl << endl;
		cout << "REngine::LoginRepository() error : Make sure your username and password are correct and match the system you are trying to log onto." << endl;

		delete g;

		return 1;
	}

	return 0;
}
int logoutRepo() {
	int iCode;

	if (!g->pEngine->logoutRepository(&iCode)) {
		cout << "REngine::logoutReository() error : " << iCode << endl;

		delete g;
		return 1;
	}
	
	return 0;
}

int login(tsNCharcb mdConnectPoint, tsNCharcb tsConnectPoint) {
	int iCode;
	LoginParams	oLoginParams;

	oLoginParams.pCallbacks = g->pCallbacks;

	oLoginParams.sMdUser.pData = user;
	oLoginParams.sMdUser.iDataLen = (int)strlen(oLoginParams.sMdUser.pData);

	oLoginParams.sMdPassword.pData = pass;
	oLoginParams.sMdPassword.iDataLen = (int)strlen(oLoginParams.sMdPassword.pData);

	oLoginParams.sUser = oLoginParams.sMdUser;
	oLoginParams.sPassword = oLoginParams.sMdPassword;

	oLoginParams.sMdCnnctPt = mdConnectPoint;

	oLoginParams.sTsCnnctPt = tsConnectPoint;

	if (!g->pEngine->login(&oLoginParams, &iCode)) {
		cout << "REngine::login() error : " << iCode << endl;

		delete g;
		return 1;
	}


	// Await Login Response
	while ((g->iMdLoginStatus != LoginStatus_Complete && g->iMdLoginStatus != LoginStatus_Failed) || 
		(g->iTsLoginStatus != LoginStatus_Complete && g->iTsLoginStatus != LoginStatus_Failed))
		Sleep(1000);

	if (g->iMdLoginStatus == LoginStatus_Failed) {
		cout << endl << endl;
		cout << "REngine::Login() error : Make sure your username and password are correct and match the system you are trying to log onto." << endl;

		delete g;
		return 1;
	}

	return 0;

}
int logout() {
	int iCode;
	if (!g->pEngine->logout(&iCode)) {
		cout << endl << endl;
		cout << "REngine::Logout() error : Honestly is this really bad?" << endl;

		delete g;
		return 1;
	}

	return 0;
}