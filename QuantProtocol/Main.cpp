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


	


	/*	========================== Check Unsigned Agreements ========================	

	iCode = Q->checkAgreements();
	if (iCode != 0) {
		delete Q;
		return (BAD);
	}
	*/

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
	Q->callbackResponses->iSelectedAccount = 0;


	/*	============================ Init List of Contracts ==========================	*/

	//Contract* ES = new Contract(Q->pEngine, Q->callbackResponses, (char*)"CME", (char*)"ESH1");
    Q->watchList.push_back(*(new Contract(Q->pEngine, Q->callbackResponses, (char*)"CME", (char*)"ESH1")));

	for (int i = 0; i < (int)Q->watchList.size(); i++)
		Q->watchList.at(0).subscribe();
	

	/*	============================= Begin Main Loop ===========================	*/

	thread bot(Analysis, Q);

	fgetc(stdin);
	

	/*	============================= Exit Main Loop ============================	*/

	Q->runtime = false;
	bot.join();
	
	cout << "Successfully made it through the program test!" << endl;
	Sleep(3000);

	/*	========================== DeInit List of Contracts =========================	*/
	for (int i = 0; i < (int)Q->watchList.size(); i++)
		Q->watchList.at(0).unsubscribe();

	/*	=========================== DeInit List of Accounts =========================	*/


	/*	========================= Logout Quant Connect Points =======================	*/

	iCode = Q->logout(false);
	if (iCode != 0) {
		delete Q;
		return (BAD);
	}

	/*	============================= Delete Quant Object ===========================	*/

	delete Q;
	return (GOOD);
}