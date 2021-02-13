#pragma once
#include "RApiPlus.h"
#include "Globals.h"
#include "Contract.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

class Order {
private:
	REngine* pEngine;
	globals* responseCallbacks;

	TradeRouteInfo* defaultRoute;
	AccountInfo* acc;

	tsNCharcb fcmId;
	tsNCharcb ibId;
	double accBalance;

public:
	Order(REngine* toEngine, globals* responses, AccountInfo* account, tsNCharcb &fcm, tsNCharcb &ib, double &balance);
	~Order();

	int init();

	int buyMarket(Contract* toBuy);
	int sellMarket(Contract* toSell);
	int shortMarket(Contract* toSell);
	int liquidate(Contract* toClose);
};