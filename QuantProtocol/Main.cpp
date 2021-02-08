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
	

	/*	============================ Init List of Contracts ==========================	*/

	//Contract* ES = new Contract(Q->pEngine, Q->callbackResponses, (char*)"CME", (char*)"ESH1");
    Q->watchList.push_back(*(new Contract(Q->pEngine, Q->callbackResponses, (char*)"CME", (char*)"ESH1")));
	Q->watchList.at(0).subscribe();
	
	for (int i = 0; i < 10; i++) {
		Q->watchList.at(0).Dom.lock();
		cout << endl << endl;
		cout << "ask is " << Q->watchList.at(0).book->priceArray[Q->watchList.at(0).book->bestAskIndex]
			<< " with size " << Q->watchList.at(0).book->askSizeArray[Q->watchList.at(0).book->bestAskIndex] << endl;
		cout << "bid is " << Q->watchList.at(0).book->priceArray[Q->watchList.at(0).book->bestBidIndex]
			<< " with size " << Q->watchList.at(0).book->bidSizeArray[Q->watchList.at(0).book->bestBidIndex] << endl;
		Q->watchList.at(0).Dom.unlock();

		Sleep(2000);
	}
	Q->watchList.at(0).unsubscribe();

	cout << "Programs actually made it this far..." << endl;
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////// FROM HERE ON IS JUST PROTOTYPING PURPOSES ///////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	//g->pEngine->subscribePnl(g->pAccounts->asAccountInfoArray + g->iSelectedAccount, &iIgnored);
	

		

	cout << "Successfully made it through the program test!" << endl;

	fgetc(stdin);

	//g->pEngine->unsubscribePnl(g->pAccounts->asAccountInfoArray + g->iSelectedAccount, &iIgnored);

	iCode = Q->logout(false);
	if (iCode != 0) {
		delete Q;
		return (BAD);
	}

	delete Q;
	return (GOOD);
}