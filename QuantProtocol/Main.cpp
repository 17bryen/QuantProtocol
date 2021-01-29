#include <iostream>
#include "Main.h"
#include "ImplRApi.h"
#include "Init.h"
#include "Logins.h"

#define GOOD 0
#define BAD 1

/*
================================================================================
This is your FINAL and VERY EXPENSIVE iteration of your automatic trading bot.
GOAL IS TO HAVE THIS BOT AUTOMATICALLY TRADE /ES CONTRACTS FROM OPEN TO CLOSE
================================================================================
*/

using namespace std;
globals::globals() {
	pAdmCallbacks = NULL;
	pCallbacks = NULL;
	pEngine = NULL;
}
globals::~globals() {
	if (this->pAdmCallbacks) {
		delete this->pAdmCallbacks;
		if (this->pCallbacks) {
			delete this->pCallbacks;
			if (this->pEngine)
				delete this->pEngine;
		}
	}
}

globals *g;

int main(int argc, char * *argv, char * *envp) {
	cout << "============================== Quant Protocol v0.1a ==============================" << endl << endl;

	/*	=============================== Declare Variables ===========================	*/
	g = new globals();

	REngineParams		oParams;
	tsNCharcb			sRepCnnctPt;
	tsNCharcb			mdCnnctPt;
	tsNCharcb			tsCnnctPt;
	char*				fake_envp[9];
	int					iCode;

	tsNCharcb			sExchange;
	tsNCharcb			sTicker;
	int					iFlags;


	iCode = initAdmCallbacks();
	if (iCode != 0) {
		cout << "Failed to init AdmCallbacks..." << endl << endl;
		return (BAD);
	}

	/*	================ Init envp to connect to Rithmic Test Server ================	*/
	{
		fake_envp[0] = (char*)"MML_DMN_SRVR_ADDR=rituz00100.00.rithmic.com:65000~rituz00100.00.rithmic.net:65000~rituz00100.00.theomne.net:65000~rituz00100.00.theomne.com:65000";
		fake_envp[1] = (char*)"MML_DOMAIN_NAME=rithmic_uat_dmz_domain";
		fake_envp[2] = (char*)"MML_LIC_SRVR_ADDR=rituz00100.00.rithmic.com:56000~rituz00100.00.rithmic.net:56000~rituz00100.00.theomne.net:56000~rituz00100.00.theomne.com:56000";
		fake_envp[3] = (char*)"MML_LOC_BROK_ADDR=rituz00100.00.rithmic.com:64100";
		fake_envp[4] = (char*)"MML_LOGGER_ADDR=rituz00100.00.rithmic.com:45454~rituz00100.00.rithmic.net:45454~rituz00100.00.theomne.com:45454~rituz00100.00.theomne.net:45454";
		fake_envp[5] = (char*)"MML_LOG_TYPE=log_net";
		// Specify the file path to the SSL file here
		fake_envp[6] = (char*)"MML_SSL_CLNT_AUTH_FILE=../QuantProtocol/Rithmic/ssl/rithmic_ssl_cert_auth_params";
		fake_envp[7] = (char*)"USER=17bryen@gmail.com";
		fake_envp[8] = NULL;
	}

	/*	======= REngineParams::sAdmCnnctPt assumes connection to Rithmic Test =======	*/
	{
		oParams.sAppName.pData = (char*)"QuantProtocol";
		oParams.sAppName.iDataLen = (int)strlen(oParams.sAppName.pData);
		oParams.sAppVersion.pData = (char*)"1.0.0.0";
		oParams.sAppVersion.iDataLen = (int)strlen(oParams.sAppVersion.pData);
		oParams.sAdmCnnctPt.pData = (char*)"dd_admin_sslc";
		oParams.sAdmCnnctPt.iDataLen = (int)strlen(oParams.sAdmCnnctPt.pData);
		oParams.envp = fake_envp;
		oParams.pAdmCallbacks = g->pAdmCallbacks;
		oParams.sLogFilePath.pData = (char*)"smd.log";
		oParams.sLogFilePath.iDataLen = (int)strlen(oParams.sLogFilePath.pData);
	}

	iCode = initREngine(oParams);
	if (iCode != 0) {
		cout << "Failed to init REngine..." << endl << endl;
		return (BAD);
	}

	iCode = initCallbacks();
	if (iCode != 0) {
		cout << "Failed to init REngine..." << endl << endl;
		return (BAD);
	}
	

	sRepCnnctPt.pData = (char*)"login_agent_repositoryc";
	sRepCnnctPt.iDataLen = (int)strlen(sRepCnnctPt.pData);

	iCode = checkAgreements(sRepCnnctPt);
	if (iCode != 0) {
		cout << "Failed to check agreements..." << endl << endl;
		return (BAD);
	}
	


	/*	============================= Login to Market/Trade Data ==========================	*/
	{
		mdCnnctPt.pData = (char*)"login_agent_tpc";
		mdCnnctPt.iDataLen = (int)strlen(mdCnnctPt.pData);

		tsCnnctPt.pData = (char*)"login_agent_opc";
		tsCnnctPt.iDataLen = (int)strlen(tsCnnctPt.pData);
	}

	iCode = login(mdCnnctPt, tsCnnctPt);
	if (iCode != 0)
		return (BAD);



	///////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////// FROM HERE ON IS JUST TESTING PURPOSES /////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////


	/*	============================ Subscribe to Market Data ===============================	

	sExchange.pData = (char*)"CME";
	sExchange.iDataLen = (int)strlen(sExchange.pData);

	sTicker.pData = (char*)"ESH";
	sTicker.iDataLen = (int)strlen(sExchange.pData);

	iFlags = (MD_PRINTS | MD_BEST);

	if (!pEngine->subscribe(&sExchange, &sTicker, iFlags, &iCode)) {
		cout << "REngine::subscribe() error : " << iCode << endl;

		delete pEngine;
		delete pCallbacks;
		delete pAdmCallbacks;

		return (BAD);
	}

	*/

	cout << "Successfully made it through the program test!" << endl;

	fgetc(stdin);

	iCode = logout();
	if (iCode != 0)
		return (BAD);

	delete g;
	return (GOOD);
}