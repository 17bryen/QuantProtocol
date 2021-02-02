#include "Logins.h"
#include "Main.h"


char* user2 = (char*)"17bryen@gmail.com";
char* pass2 = (char*)"gDKI8QoY";

char* user = (char*)"17bryen@amp.com";
char* pass = (char*)"&h$QlbrU2ha";

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

int login(tsNCharcb* ihConnectPoint, tsNCharcb* mdConnectPoint, tsNCharcb* tsConnectPoint, tsNCharcb* plConnectPoint) {
	int iCode;
	LoginParams	oLoginParams;

	g->pEngine->logout(&iCode);
	Sleep(1000);

	oLoginParams.pCallbacks = g->pCallbacks;
	
	if (ihConnectPoint != NULL) {
		oLoginParams.sIhCnnctPt = *ihConnectPoint;

		oLoginParams.sIhUser.pData = user;
		oLoginParams.sIhUser.iDataLen = (int)strlen(oLoginParams.sIhUser.pData);

		oLoginParams.sIhPassword.pData = pass;
		oLoginParams.sIhPassword.iDataLen = (int)strlen(oLoginParams.sIhPassword.pData);
	}

	if (mdConnectPoint != NULL) {
		oLoginParams.sMdCnnctPt = *mdConnectPoint;

		oLoginParams.sMdUser.pData = user;
		oLoginParams.sMdUser.iDataLen = (int)strlen(oLoginParams.sMdUser.pData);

		oLoginParams.sMdPassword.pData = pass;
		oLoginParams.sMdPassword.iDataLen = (int)strlen(oLoginParams.sMdPassword.pData);
	}
	
	if (tsConnectPoint != NULL || plConnectPoint != NULL) {
		oLoginParams.sUser.pData = user;
		oLoginParams.sUser.iDataLen = (int)strlen(oLoginParams.sUser.pData);

		oLoginParams.sPassword.pData = pass;
		oLoginParams.sPassword.iDataLen = (int)strlen(oLoginParams.sPassword.pData);

		if (tsConnectPoint != NULL) {
			oLoginParams.sTsCnnctPt = *tsConnectPoint;
		}

		if (plConnectPoint != NULL) {
			oLoginParams.sPnlCnnctPt = *plConnectPoint;
		}
	}
	

	if (!g->pEngine->login(&oLoginParams, &iCode)) {
		cout << "REngine::login() error : " << iCode << endl;

		delete g;
		return 1;
	}


	// Await Login Response
	if (mdConnectPoint != NULL)
		while (g->iMdLoginStatus != LoginStatus_Complete && g->iMdLoginStatus != LoginStatus_Failed && g->iMdLoginStatus != LoginStatus_AwaitingResults)
			Sleep(1000);
		
	if (tsConnectPoint != NULL)
		while (g->iTsLoginStatus != LoginStatus_Complete && g->iTsLoginStatus != LoginStatus_Failed && g->iTsLoginStatus != LoginStatus_AwaitingResults)
			Sleep(1000);

	if (plConnectPoint != NULL)
		while (g->iPnlLoginStatus != LoginStatus_Complete && g->iPnlLoginStatus != LoginStatus_Failed && g->iPnlLoginStatus != LoginStatus_AwaitingResults)
			Sleep(1000);

	if (ihConnectPoint != NULL)
		while (g->iIhLoginStatus != LoginStatus_Complete && g->iIhLoginStatus != LoginStatus_Failed && g->iIhLoginStatus != LoginStatus_AwaitingResults)
			Sleep(1000);

	if (g->iMdLoginStatus == LoginStatus_Failed || 
		g->iTsLoginStatus == LoginStatus_Failed || 
		g->iPnlLoginStatus == LoginStatus_Failed || 
		g->iIhLoginStatus == LoginStatus_Failed) {
		cout << endl << endl;
		cout << "REngine::Login() error : Make sure your username and password are correct and match the system you are trying to log onto." << endl;

		delete g;
		return 1;
	}
	else if (g->iMdLoginStatus == LoginStatus_AwaitingResults ||
		g->iTsLoginStatus == LoginStatus_AwaitingResults ||
		g->iPnlLoginStatus == LoginStatus_AwaitingResults ||
		g->iIhLoginStatus == LoginStatus_AwaitingResults) {
		cout << endl << endl;
		cout << "1 or more connection services broken... awaiting reconnect." << endl;

		while (g->iMdLoginStatus == LoginStatus_AwaitingResults ||
			g->iTsLoginStatus == LoginStatus_AwaitingResults ||
			g->iPnlLoginStatus == LoginStatus_AwaitingResults ||
			g->iIhLoginStatus == LoginStatus_AwaitingResults) {
			Sleep(10000);

			cout << "Attempting another login..." << endl;
			g->pEngine->login(&oLoginParams, &iCode);
		}
	
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