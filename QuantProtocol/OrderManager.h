#pragma once

#include "Systems.h"

class OrderManager {
private:
	REngine* pEngine;
	Systems* pSystem;

	TradeRouteListInfo* defaultRoutes;

public:
	OrderManager(REngine* toEngine, Systems* toSystem);
	~OrderManager();

	int init();

	int buyMarket(Contract* toBuy);
	int sellMarket(Contract* toSell);
	int shortMarket(Contract* toSell);
	int liquidate(Contract* toClose);
};