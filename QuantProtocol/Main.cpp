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

	Q->pSystem->setUser((char*)"17bryen@amp.com");
	Q->pSystem->setPass((char*)"&h$QlbrU2ha");

	/*	========================== Check Unsigned Agreements ========================	

	iCode = Q->system->checkAgreements();
	if (iCode != 0) {
		delete Q;
		return (BAD);
	}
	*/

	/*	======================== Login to Quant Connect Points ======================	*/

	Q->pSystem->init();
	iCode = Q->pSystem->login();
	if (iCode != 0) {
		delete Q;
		return (BAD);
	}

	/*	================================ Init Account ===============================	*/

	Q->pSystem->acc->initAcc();
	Q->pSystem->acc->subscribe();

	/*	============================ Init List of Contracts ==========================	*/

    Q->pSystem->acc->addWatchlist((char*)"CME", (char*)"ESH1");
	Q->pSystem->acc->subWatchlist();

	/*	============================= Begin Main Loop ===========================	*/

	Q->initThreads();

	//thread analyManager(&AnalysisManager::Analysis, Q->analysisManager);
	thread orderManager(&OrderManager::Order, Q->orderManager);
	//thread stateManager(Q->stateManager, system);

	//Q->orderManager->buyMarket(0);

	cout << "Successfully made it through the program test!" << endl;
	fgetc(stdin);
	

	/*	============================= Exit Main Loop ============================	*/

	Q->pSystem->userManagement = false;
	//analyManager.join();
	orderManager.join();
	//stateManager.join();
	
	cout << "Successfully made it through the program test!" << endl;
	Sleep(3000);

	/*	========================== DeInit List of Contracts =========================	*/

	Q->pSystem->acc->unsubWatchlist();
	Q->pSystem->acc->deleteWatchlist();

	/*	=========================== DeInit List of Accounts =========================	*/

	Q->pSystem->acc->unsubscribe();

	/*	========================= Logout Quant Connect Points =======================	*/

	iCode = Q->pSystem->logout(false);
	if (iCode != 0) {
		delete Q;
		return (BAD);
	}

	/*	============================= Delete Quant Object ===========================	*/

	delete Q;
	return (GOOD);
}