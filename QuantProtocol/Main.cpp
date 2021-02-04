#include <iostream>
#include "Main.h"

#define GOOD 0
#define BAD 1

/*
================================================================================
This is your FINAL and VERY EXPENSIVE iteration of your automatic trading bot.
GOAL IS TO HAVE THIS BOT AUTOMATICALLY TRADE /ES CONTRACTS FROM OPEN TO CLOSE
================================================================================
*/

using namespace std;



int main(int argc, char * *argv, char * *envp) {
	cout << "============================== Quant Protocol v0.1a ==============================" << endl << endl;

	/*	=============================== Declare Variables ===========================	*/

	Quant*				Q;
	REngineParams		oParams;
	int					iCode;

	tsNCharcb			sExchange;
	tsNCharcb			sTicker;
	int					iFlags;


	/*	======= REngineParams::sAdmCnnctPt assumes connection to Rithmic 01 ==========	*/
	{
		oParams.sAppName.pData = (char*)"brro:QuantCLI";
		oParams.sAppName.iDataLen = (int)strlen(oParams.sAppName.pData);
		oParams.sAppVersion.pData = (char*)"1.2.0.0";
		oParams.sAppVersion.iDataLen = (int)strlen(oParams.sAppVersion.pData);
		oParams.sAdmCnnctPt.pData = (char*)"dd_admin_sslc";
		oParams.sAdmCnnctPt.iDataLen = (int)strlen(oParams.sAdmCnnctPt.pData);
		oParams.envp = NULL;
		oParams.pAdmCallbacks = NULL;
		oParams.sLogFilePath.pData = (char*)"smd.log";
		oParams.sLogFilePath.iDataLen = (int)strlen(oParams.sLogFilePath.pData);
	}


	/*	======================== Create and Init Quant Object =======================	*/
	
	Q = new Quant();
	iCode = Q->init(oParams);
	if (iCode != 0) {
		delete Q;
		return (BAD);
	}


	/*	========================= Pass Rithmic Login to Quant =======================	*/


	/*	========================== Check Unsigned Agreements ========================	*/

	iCode = Q->checkAgreements();
	if (iCode != 0) {
		delete Q;
		return (BAD);
	}


	/*	======================== Login to Quant Connect Points ======================	*/

	iCode = Q->login();
	if (iCode != 0) {
		delete Q;
		return (BAD);
	}


	/*	============================ Get List of Accounts ===========================	*/

	
	while (!Q->callbackResponses->bRcvdAccountsList) {
		Sleep(1000);
	}
	

	cout << "Programs actually made it this far..." << endl;
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////// FROM HERE ON IS JUST PROTOTYPING PURPOSES ///////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	//g->pEngine->subscribePnl(g->pAccounts->asAccountInfoArray + g->iSelectedAccount, &iIgnored);
	

	/*	============================ Subscribe to Market Data ===============================	

	sExchange.pData = (char*)"CME";
	sExchange.iDataLen = (int)strlen(sExchange.pData);

	sTicker.pData = (char*)"ESH1";
	sTicker.iDataLen = (int)strlen(sTicker.pData);

	
	iFlags = (MD_PRINTS | MD_BEST);

	if (!g->pEngine->subscribe(&sExchange, &sTicker, iFlags, &iCode)) {
		cout << "REngine::subscribe() error : " << iCode << endl;

		delete g;
		return (BAD);
	}
	
	*/
	/*
	sExchange.pData = (char*)"CME";
	sExchange.iDataLen = (int)strlen(sExchange.pData);

	sTicker.pData = (char*)"ESH1";
	sTicker.iDataLen = (int)strlen(sTicker.pData);

	try {
		Q->pEngine->replayTrades(&sExchange, &sTicker, 1611855000, 1611855060, &iCode);
		cout << endl << "Successfully called replayTrades with code: " << iCode << endl;
	}
	catch (OmneException& oEx) {
		delete Q;

		cout << "REngin::ReplayTrades() error : " << iCode << endl;
		return 1;
	}

	while (!Q->callbackResponses->bRcvdReplayTrades)
		Sleep(1000);
	*/	
		

	cout << "Successfully made it through the program test!" << endl;


	fgetc(stdin);

	/*
	
	if (!g->pEngine->unsubscribe(&sExchange, &sTicker, &iCode)) {
		cout << "REngine::unsubscribe() error : " << iCode << endl;

		delete g;
		return (BAD);
	}
	*/

	//g->pEngine->unsubscribePnl(g->pAccounts->asAccountInfoArray + g->iSelectedAccount, &iIgnored);

	iCode = Q->logout(false);
	if (iCode != 0) {
		delete Q;
		return (BAD);
	}

	delete Q;
	return (GOOD);
}