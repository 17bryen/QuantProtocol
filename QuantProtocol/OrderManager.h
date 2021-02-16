#pragma once

#include "Systems.h"

class OrderManager {
private:
	REngine* pEngine;
	Systems* pSystem;

	AccountInfo* account;
	vector<Contract*>* watchlist;

	TradeRouteListInfo* defaultRoutes;

public:
	OrderManager(REngine* toEngine, Systems* toSystem);
	~OrderManager();

	int init();
	int order();

	int buyMarket(int toBuyIndex);
	int sellMarket(int toSellIndex);
	int liquidate(int toCloseIndex);
};