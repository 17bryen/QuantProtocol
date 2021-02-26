#include "OrderManager.h"

/*	==========================================================================	*/
/*								Define Constructors								*/
/*	==========================================================================	*/

OrderManager::OrderManager(REngine* toEngine, Systems* toSystem) {
	pEngine = toEngine;
	pSystem = toSystem;

	account = pSystem->acc->account;
	watchlist = pSystem->acc->watchlist;

	defaultRoutes = new TradeRouteListInfo();
}
OrderManager::~OrderManager() {

}

/*	==========================================================================	*/
/*								Define Member Functions							*/
/*	==========================================================================	*/

int OrderManager::init() {
	int iCode;

	/*
	try {
		pEngine->listTradeRoutes(NULL, &iCode);
	}
	catch (OmneException oEx) {
		cout << endl << "REngine::listTradeRoutes() error : " << iCode << endl;
		return 1;
	}

	while (!responseCallbacks->bRcvdTradeRoutes)
		Sleep(1000);
	
	for (int i = 0; i < tradeRoutes->iArrayLen; i++) {
		if ((char)*tradeRoutes->asTradeRouteInfoArray[i].sDefault.pData == 'Y') {
			defaultRoute = &tradeRoutes->asTradeRouteInfoArray[i];
			break;
		}
	}

	if (defaultRoute == nullptr) {
		cout << endl << "Could not find default order route..." << endl;
		return 1;
	}
	*/

	try {
		pEngine->subscribeTradeRoute(&account->sFcmId, &account->sIbId, (void*)pSystem, &iCode);
	}
	catch (OmneException oEx) {
		cout << endl << "REngine::subscribeTradeRoutes() error : " << iCode << endl;
		return 1;
	}

	//DETERMINE HOW TO SELECT CME ROUTE HERE

	return 0;
}
int OrderManager::deinit() {
	int iCode;

	try {
		pEngine->unsubscribeTradeRoute(&account->sFcmId, &account->sIbId, &iCode);
	}
	catch (OmneException oEx) {
		cout << endl << "REngine::subscribeTradeRoutes() error : " << iCode << endl;
		return 1;
	}

	//DETERMINE HOW TO SELECT CME ROUTE HERE

	return 0;
}

int OrderManager::buyMarket(int toBuyIndex) {
	int iCode;
	MarketOrderParams toSubmit = MarketOrderParams();

	toSubmit.iQty = 0; //Create logic to calculate size based on risk and capital

	toSubmit.pAccount = account;
	toSubmit.sExchange = watchlist->at(toBuyIndex)->exchange;
	toSubmit.sTicker = watchlist->at(toBuyIndex)->ticker;

	toSubmit.pContext = watchlist->at(toBuyIndex);
	//toSubmit.sRoutingInstructions = //?;
	toSubmit.sBuySellType = sBUY_SELL_TYPE_BUY;
	toSubmit.sDuration = sORDER_DURATION_DAY;
	toSubmit.sEntryType = sORDER_ENTRY_TYPE_AUTO;

	/*
	try {
		pEngine->sendOrder(&toSubmit, &iCode);
	}
	catch (OmneException oEx) {
		cout << endl << "REnging::sendOrder( MarketOrderParams ) error : " << iCode << endl;
		return 1;
	}
	*/

	return 0;
}
int OrderManager::sellMarket(int toSellIndex) {
	int iCode;
	MarketOrderParams toSubmit = MarketOrderParams();

	toSubmit.iQty = 0; //Create logic to calculate size based on risk and capital

	toSubmit.pAccount = account;
	toSubmit.sExchange = watchlist->at(toSellIndex)->exchange;
	toSubmit.sTicker = watchlist->at(toSellIndex)->ticker;

	toSubmit.pContext = watchlist->at(toSellIndex);
	//toSubmit.sRoutingInstructions = //?;
	toSubmit.sBuySellType = sBUY_SELL_TYPE_SELL;
	toSubmit.sDuration = sORDER_DURATION_DAY;
	toSubmit.sEntryType = sORDER_ENTRY_TYPE_AUTO;

	/*
	try {
		pEngine->sendOrder(&toSubmit, &iCode);
	}
	catch (OmneException oEx) {
		cout << endl << "REnging::sendOrder( MarketOrderParams ) error : " << iCode << endl;
		return 1;
	}
	*/

	return 0;
}
int OrderManager::liquidate(int toCloseIndex) {
	int iCode;
	MarketOrderParams toSubmit = MarketOrderParams();

	toSubmit.iQty = watchlist->at(toCloseIndex)->positionSize;
	if (toSubmit.iQty > 0)
		toSubmit.sBuySellType = sBUY_SELL_TYPE_SELL;
	else if (toSubmit.iQty < 0)
		toSubmit.sBuySellType = sBUY_SELL_TYPE_BUY;
	else {
		cout << endl << "Can not liquidate a nonexistent position!" << endl;
		return 1;
	}

	toSubmit.pAccount = account;
	toSubmit.sExchange = watchlist->at(toCloseIndex)->exchange;
	toSubmit.sTicker = watchlist->at(toCloseIndex)->ticker;

	toSubmit.pContext = watchlist->at(toCloseIndex);
	//toSubmit.sRoutingInstructions = //?;
	toSubmit.sDuration = sORDER_DURATION_DAY;
	toSubmit.sEntryType = sORDER_ENTRY_TYPE_AUTO;

	/*
	try {
		pEngine->sendOrder(&toSubmit, &iCode);
	}
	catch (OmneException oEx) {
		cout << endl << "REnging::sendOrder( MarketOrderParams ) error : " << iCode << endl;
		return 1;
	}
	*/

	return 0;
}

/*	==========================================================================	*/
/*								Main Order Loop									*/
/*	==========================================================================	*/

int OrderManager::Order() {
	int iCode;
	cout << endl << "Beginning Order Management with account " << ((string)pSystem->acc->account->sAccountName.pData).substr(0, pSystem->acc->account->sAccountName.iDataLen) << " with balance $" << pSystem->acc->accBalance << endl;
	
	if (!pEngine->replayOpenOrders(account, &iCode))
		cout << endl << "REngine::replayOpenOrders() error : " << iCode << endl;
	

	while (pSystem->userManagement) {
		for (int i = 0; i < watchlist->size(); i++) {
			if (watchlist->at(i)->placeBuyOrder)
				buyMarket(i);
			else if (watchlist->at(i)->placeSellOrder)
				sellMarket(i);
		}

		Sleep(1000);
	}

	return 0;
}
