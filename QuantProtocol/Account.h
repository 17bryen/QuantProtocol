#pragma once

#include "Contract.h"

class Account {
private:
	REngine* pEngine;
public:
	/*	======================== Declare Member Variables ========================	*/
	double accBalance;

	AccountInfo* account;
	vector<Contract*>* watchlist;

	/*	======================== Declare Member Functions ========================	*/
	Account();
	Account(REngine* engine);
	~Account();

	int initAcc();
	int subscribe();
	int unsubscribe();

	int addWatchlist(char* toTicker, char* toExchange);
	int subWatchlist();
	int unsubWatchlist();
	int deleteWatchlist();

	bool setEngine(REngine* toEngine);
};