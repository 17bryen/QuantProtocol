#include "Order.h"

Order::Order(REngine* toEngine, globals* responses, AccountInfo* account, tsNCharcb& fcm, tsNCharcb& ib, double& balance) {
	pEngine = toEngine;
	responseCallbacks = responses;

	acc = account;
	fcmId = fcm;
	ibId = ib;

	defaultRoute = nullptr;
	accBalance = balance;
}
Order::~Order() {
	if (defaultRoute != nullptr)
		delete defaultRoute;
}

int Order::init() {
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
		pEngine->subscribeTradeRoute(&fcmId, &ibId, NULL, &iCode);
	}
	catch (OmneException oEx) {
		cout << endl << "REngine::subscribeTradeRoutes() error : " << iCode << endl;
		return 1;
	}

	//DETERMINE HOW TO SELECT CME ROUTE HERE


}

int Order::buyMarket(Contract* toBuy) {
	int iCode;
	MarketOrderParams toSubmit = MarketOrderParams();

	toSubmit.iQty = 0; //Create logic to calculate size based on risk and capital

	toSubmit.pAccount = acc;
	toSubmit.sExchange = toBuy->exchange;
	toSubmit.sTicker = toBuy->ticker;

	toSubmit.pContext = NULL;
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
int Order::sellMarket(Contract* toSell) {
	int iCode;
	MarketOrderParams toSubmit = MarketOrderParams();

	toSubmit.iQty = 0; //Create logic to calculate size based on risk and capital

	toSubmit.pAccount = acc;
	toSubmit.sExchange = toSell->exchange;
	toSubmit.sTicker = toSell->ticker;

	toSubmit.pContext = NULL;
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
int Order::shortMarket(Contract* toSell) {
	int iCode;
	MarketOrderParams toSubmit = MarketOrderParams();

	toSubmit.iQty = 0; //Create logic to calculate size based on risk and capital

	toSubmit.pAccount = acc;
	toSubmit.sExchange = toSell->exchange;
	toSubmit.sTicker = toSell->ticker;

	toSubmit.pContext = NULL;
	//toSubmit.sRoutingInstructions = //?;
	toSubmit.sBuySellType = sBUY_SELL_TYPE_SELL_SHORT;
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
int Order::liquidate(Contract* toClose) {
	int iCode;
	MarketOrderParams toSubmit = MarketOrderParams();

	toSubmit.iQty = toClose->position;
	if (toSubmit.iQty > 0)
		toSubmit.sBuySellType = sBUY_SELL_TYPE_SELL;
	else if (toSubmit.iQty < 0)
		toSubmit.sBuySellType = sBUY_SELL_TYPE_BUY;
	else {
		cout << endl << "Can not liquidate a nonexistent position!" << endl;
		return 1;
	}

	toSubmit.pAccount = acc;
	toSubmit.sExchange = toClose->exchange;
	toSubmit.sTicker = toClose->ticker;

	toSubmit.pContext = NULL;
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

