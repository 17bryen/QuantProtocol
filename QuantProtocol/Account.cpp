#include "Account.h"

Account::Account() {
	pEngine = nullptr;

	accBalance = 2300;

	account = new AccountInfo();
	watchlist = new vector<Contract>;
	watchlist->reserve(3);
}
Account::Account(REngine* engine) {
	pEngine = engine;

	accBalance = 2300;

	account = new AccountInfo();
	watchlist = new vector<Contract>;
	watchlist->reserve(3);
}
Account::~Account() {
	
}

/*	=========================================================================	*/

int Account::initAcc() {
	//CALL REPLAY PNL HERE
	return 0;
}
int Account::subscribe() {
	int iCode;
	try {
		pEngine->subscribePnl(account, &iCode);
	}
	catch (OmneException &oEx){
		cout << endl << "Failed to subscribe to PnL server for account ID " << account->sAccountId.pData
			<< ";  ERRORS: " << iCode << endl;
		return 1;
	}
	return 0;
}
int Account::unsubscribe() {
	int iCode;
	pEngine->unsubscribePnl(account, &iCode);
	return 1;
}

/*	=========================================================================	*/

int Account::addWatchlist(char* toExchange, char* toTicker) {
	watchlist->push_back(new Contract(pEngine, toExchange, toTicker));
	return 0;
}
int Account::subWatchlist() {
	for (int i = 0; i < watchlist->size(); i++)
		watchlist->at(0)->subscribe();
}

bool Account::setEngine(REngine* toEngine) {
	pEngine = toEngine;
	return true;
}