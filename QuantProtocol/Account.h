#pragma once
#include "RApiPlus.h"
#include "Globals.h"
#include "Contract.h"
#include "Order.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <mutex>

class Account {
public:
	/*	======================== Declare Member Variables ========================	*/
	tsNCharcb fcmId;
	tsNCharcb ibId;
	tsNCharcb accName;
	tsNCharcb accId;

	double accBalance;
	Order* order;
	TradeRouteListInfo* tradeRoutes;

	AccountInfo* account;
	REngine* pEngine;
	globals* responseCallbacks;

	/*	======================== Declare Member Functions ========================	*/
	Account();
	Account(AccountInfo* acc);
	Account(REngine* engine, globals* responses);
	Account(REngine* engine, globals* responses, AccountInfo* acc);
	~Account();

	int initAcc();
	int subscribe();
	int unsubscribe();

};