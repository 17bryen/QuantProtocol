#include "Contract.h"

using namespace std;
using namespace RApi;

int getCurrentTime() {
	int toReturn = (chrono::duration_cast<chrono::seconds>(chrono::system_clock::now().time_since_epoch())).count();
	//DONT? ACCOUNT FOR TIME DIFFERENCE (times in GMT; KW 3 hours ahead GMT; KW 9 ahead CST) (Already returns UTC which = GMT)
	//toReturn -= (60 * 60 * 9);

	return toReturn;
}

/*   =====================================================================   */
/*                          Contract Definitions                             */
/*   =====================================================================   */

Contract::Contract(REngine* toEngine, char* toExchange, char* toTicker) {
    pEngine = toEngine;

	exchange.pData = toExchange;
	exchange.iDataLen = (int)strlen(exchange.pData);

	ticker.pData = toTicker;
	ticker.iDataLen = (int)strlen(ticker.pData);

	book = new OrderBook();
	flow = new OrderFlow();
	pending = nullptr;

	positionSize = 0;

	rcvdLimitOrderBook = false;
	rcvdReplayTrades = false;
}

Contract::Contract(const Contract &orig) {
	domLock.lock();
	tapeLock.lock();

	pEngine = orig.pEngine;

	book = orig.book;
	flow = orig.flow;
	pending = orig.pending;

	exchange.pData = orig.exchange.pData;
	exchange.iDataLen = orig.exchange.iDataLen;

	ticker.pData = orig.ticker.pData;
	ticker.iDataLen = orig.ticker.iDataLen;

	positionSize = 0;

	rcvdLimitOrderBook = orig.rcvdLimitOrderBook;
	rcvdReplayTrades = orig.rcvdReplayTrades;

	tapeLock.unlock();
	domLock.unlock();
}

Contract::~Contract() {
	delete book;
	delete flow;
	if (pending != nullptr)
		delete pending;
}

/*   =====================================================================   */

int Contract::subscribe() {
    int iCode;
	int iFlags = (MD_QUOTES | MD_PRINTS);
	int timeNow = getCurrentTime();
	int timeBack = timeNow - (60 * 1);

	/*
	try {
		pEngine->suspendInput(&iCode);
	}
	catch (OmneException oEx) {
		cout << endl << "REngine::suspendInput() error : " << iCode << endl;
		return 1;
	}
	*/

	if (!pEngine->subscribe(&exchange, &ticker, iFlags, &iCode)) {
		cout << "REngine::subscribe() error : " << iCode << endl;

		return (1);
	}
	if (!pEngine->replayTrades(&exchange, &ticker, timeBack, timeNow, &iCode)) {
		cout << "REngine::replayTrades() error : " << iCode << endl;

		return (1);
	}

	//Wait for past hour of trades to replay before continuing
	cout << "Waiting on past hour of trade data before continuing..." << endl;
	while (!rcvdReplayTrades) {
		Sleep(1000);
	}

    //Wait for Book image received before continuing
	//while (!rcvdLimitOrderBook)
		Sleep(1000);

    return 0;
}

int Contract::unsubscribe() {
    int iCode;
    if (!pEngine->unsubscribe(&exchange, &ticker, &iCode)) {
		cout << "REngine::unsubscribe() error : " << iCode << endl;
		return 1;
	}

    return 0;
}

/*   =====================================================================   */
/*                          OrderBook definitions                            */
/*   =====================================================================   */


OrderBook::OrderBook() {
	priceArray = nullptr;
	timeArray = nullptr;
	domLength = 0;

	bestAskIndex = 0;
	askOrderArray = nullptr;
	askSizeArray = nullptr;

	bestBidIndex = 0;
	bidOrderArray = nullptr;
	bidSizeArray = nullptr;
}
OrderBook::~OrderBook() {
	delete priceArray;

	delete askOrderArray;
	delete askSizeArray;

	delete bidOrderArray;
	delete bidSizeArray;
}

/*   =====================================================================   */

int OrderBook::updateBook(LimitOrderBookInfo* tick) {

	domLength = tick->iAskArrayLen + tick->iBidArrayLen;

	priceArray = new double[domLength];

	bestAskIndex = tick->iBidArrayLen;
	askOrderArray = new int[domLength];
	askSizeArray = new int[domLength];

	bestBidIndex = tick->iBidArrayLen - 1;
	bidOrderArray = new int[domLength];
	bidSizeArray = new int[domLength];

	for (int i = 0; i < domLength; i++) {
		askOrderArray[i] = 0;
		bidOrderArray[i] = 0;
		askSizeArray[i] = 0;
		bidSizeArray[i] = 0;
	}

	for (int i = bestBidIndex; i >= 0; i--) {
		priceArray[bestBidIndex - i] = tick->adBidPriceArray[i];
		bidOrderArray[bestBidIndex - i] = tick->aiBidNumOrdersArray[i];
		bidSizeArray[bestBidIndex - i] = tick->aiBidSizeArray[i];
	}


	for (int i = bestAskIndex; i < domLength; i++) {
		priceArray[i] = tick->adAskPriceArray[i - bestAskIndex];
		askOrderArray[i] = tick->aiAskNumOrdersArray[i - bestAskIndex];
		askSizeArray[i] = tick->aiAskSizeArray[i - bestAskIndex];
	}

	return 0;
}

int OrderBook::updateAsk(AskInfo* tick) {
	bool added = false;

    if (tick->bPriceFlag && tick->bSizeFlag) {
		while (tick->dPrice < priceArray[bestAskIndex]) {
			if (--bestAskIndex == bestBidIndex)
				bestBidIndex--;
		}
		while (bidOrderArray[bestBidIndex] <= 0)
			bestBidIndex--;
        for (int i = bestAskIndex; i < domLength; i++)
			if (priceArray[i] == tick->dPrice) {
				askOrderArray[i] = tick->iNumOrders;
				askSizeArray[i] = tick->iSize;
				added = true;
			}
	}
	else
		cout << endl << "Ask update to Orderbook did not contain enough info..." << endl;

	if (!added) {
		int insertIndex = -1;

		for (int i = bestBidIndex; i < domLength; i++) {
			if (priceArray[i] > tick->dPrice && priceArray[i - 1] < tick->dPrice) {
				insertIndex = i;
				if (insertIndex < bestAskIndex)
					bestAskIndex++;

				double* newPriceArray = new double[domLength + 1];

				int* newAskOrderArray = new int[domLength + 1];
				int* newAskSizeArray = new int[domLength + 1];

				int* newBidOrderArray = new int[domLength + 1];
				int* newBidSizeArray = new int[domLength + 1];

				for (int j = 0; j < insertIndex; j++) {
					newPriceArray[j] = priceArray[j];
					newAskOrderArray[j] = askOrderArray[j];
					newAskSizeArray[j] = askSizeArray[j];
					newBidOrderArray[j] = bidOrderArray[j];
					newBidSizeArray[j] = bidSizeArray[j];
				}

				newPriceArray[insertIndex] = tick->dPrice;
				newAskOrderArray[insertIndex] = tick->iNumOrders;
				newAskSizeArray[insertIndex] = tick->iSize;
				newBidOrderArray[insertIndex] = 0;
				newBidSizeArray[insertIndex] = 0;

				for (int j = insertIndex + 1; j < domLength + 1; j++) {
					newPriceArray[j] = priceArray[j - 1];
					newAskOrderArray[j] = askOrderArray[j - 1];
					newAskSizeArray[j] = askSizeArray[j - 1];
					newBidOrderArray[j] = bidOrderArray[j - 1];
					newBidSizeArray[j] = bidSizeArray[j - 1];
				}

				domLength++;
				priceArray = newPriceArray;
				askOrderArray = newAskOrderArray;
				askSizeArray = newAskSizeArray;
				bidOrderArray = newBidOrderArray;
				bidSizeArray = newBidSizeArray;
			}
		}
		if (insertIndex == -1) {
			cout << endl << "could not find ask array price update: " << tick->dPrice << endl;
			return 1;
		}
		else {
			cout << endl << "inserted ask array price update at: " << tick->dPrice << endl;
		}
	}
		
	return 0;
}
int OrderBook::updateBid(BidInfo* tick) {
	bool added = false;

	if (tick->bPriceFlag && tick->bSizeFlag) {
		while (tick->dPrice > priceArray[bestBidIndex]) {
			if (++bestBidIndex == bestAskIndex)
				bestAskIndex++;
		}
		while (askOrderArray[bestAskIndex] <= 0)
			bestAskIndex++;
		for (int i = bestBidIndex; i >= 0; i--)
			if (priceArray[i] == tick->dPrice) {
				bidOrderArray[i] = tick->iNumOrders;
				bidSizeArray[i] = tick->iSize;
				added = true;
			}
	}
	else
		cout << endl << "Bid update to Orderbook did not contain enough info..." << endl;

	if (!added) {
		int insertIndex = -1;

		for (int i = bestAskIndex; i > 0; i--) {
			if (priceArray[i] > tick->dPrice && priceArray[i - 1] < tick->dPrice) {
				insertIndex = i;
				if (insertIndex < bestBidIndex)
					bestBidIndex++;
				if (insertIndex < bestAskIndex)
					bestAskIndex++;

				double* newPriceArray = new double[domLength + 1];

				int* newAskOrderArray = new int[domLength + 1];
				int* newAskSizeArray = new int[domLength + 1];

				int* newBidOrderArray = new int[domLength + 1];
				int* newBidSizeArray = new int[domLength + 1];

				for (int j = 0; j < insertIndex; j++) {
					newPriceArray[j] = priceArray[j];
					newAskOrderArray[j] = askOrderArray[j];
					newAskSizeArray[j] = askSizeArray[j];
					newBidOrderArray[j] = bidOrderArray[j];
					newBidSizeArray[j] = bidSizeArray[j];
				}

				newPriceArray[insertIndex] = tick->dPrice;
				newAskOrderArray[insertIndex] = 0;
				newAskSizeArray[insertIndex] = 0;
				newBidOrderArray[insertIndex] = tick->iNumOrders;
				newBidSizeArray[insertIndex] = tick->iSize;

				for (int j = insertIndex + 1; j < domLength + 1; j++) {
					newPriceArray[j] = priceArray[j - 1];
					newAskOrderArray[j] = askOrderArray[j - 1];
					newAskSizeArray[j] = askSizeArray[j - 1];
					newBidOrderArray[j] = bidOrderArray[j - 1];
					newBidSizeArray[j] = bidSizeArray[j - 1];
				}

				domLength++;
				priceArray = newPriceArray;
				askOrderArray = newAskOrderArray;
				askSizeArray = newAskSizeArray;
				bidOrderArray = newBidOrderArray;
				bidSizeArray = newBidSizeArray;
			}
		}
		if (insertIndex == -1) {
			cout << endl << "could not find bid array price update: " << tick->dPrice << endl;
			return 1;
		}
		else {
			cout << endl << "inserted bid array price update at: " << tick->dPrice << endl;
		}
	}

	return 0;
}

/*   =====================================================================   */
/*                          OrderFlow definitions                            */
/*   =====================================================================   */


OrderFlow::OrderFlow() {
	priceArray = {};
	priceArrayLength = 0;

	recAskVolArray = {};
	askVolumeArray = {};

	recBidVolArray = {};
	bidVolumeArray = {};

	recTime = {};

	tradeFilter = new vector<Trade*>;
	tradeFilter->reserve(21);
}
OrderFlow::~OrderFlow() {
	delete priceArray;
	delete askVolumeArray;
	delete recAskVolArray;
	delete bidVolumeArray;
	delete recBidVolArray;
}

/*   =====================================================================   */

int OrderFlow::updateTrades(TradeInfo* tick) {
	if (!tick->bPriceFlag) {
		return 1;
	}
	//Trade Filter
	if (tick->iSize > 20) {
		Trade temp;
		temp.price = tick->bPriceFlag;
		temp.size = tick->iSize;
		temp.time = tick->iSsboe;
		temp.aggrSide = (char)*tick->sAggressorSide.pData;
		tradeFilter->insert(tradeFilter->begin(), &temp);
		if (tradeFilter->size() > 20)
			tradeFilter->pop_back();
	}

	//Volume record
	int priceIndex = findPriceIndex(tick->dPrice);
	if (priceIndex == -1)
		priceIndex = insertPrice(tick->dPrice);

	if (tick->iSsboe - recTime[priceIndex] > 60) {
		recAskVolArray[priceIndex] = 0;
		recBidVolArray[priceIndex] = 0;
	}

	recTime[priceIndex] = tick->iSsboe;

	if ((char)*tick->sAggressorSide.pData == 'B') {
		recAskVolArray[priceIndex] += tick->iSize;
		askVolumeArray[priceIndex] += tick->iSize;
	} 
	else if ((char)*tick->sAggressorSide.pData == 'S') {
		recBidVolArray[priceIndex] += tick->iSize;
		bidVolumeArray[priceIndex] += tick->iSize;
	}

	return 0;
}
int OrderFlow::findPriceIndex(double toFind) {
	int index = -1;
	for (int i = 0; i < priceArrayLength; i++)
		if (priceArray[i] == toFind) {
			index = i;
			break;
		}
	return index;
}
int OrderFlow::insertPrice(double toInsert) {
	int insertIndex = 0;
	double* newPriceArray = new double[priceArrayLength + 1];
	int* newRecAskVol = new int[priceArrayLength + 1];
	int* newAskVol = new int[priceArrayLength + 1];
	int* newRecBidVol = new int[priceArrayLength + 1];
	int* newBidVol = new int[priceArrayLength + 1];
	int* newRecTime = new int[priceArrayLength + 1];

	for (int i = 0; i < priceArrayLength; i++)
		if (priceArray[i] < toInsert)
			insertIndex = i + 1;

	for (int i = 0; i < insertIndex; i++) {
		newPriceArray[i] = priceArray[i];
		newRecAskVol[i] = recAskVolArray[i];
		newAskVol[i] = askVolumeArray[i];
		newRecBidVol[i] = recBidVolArray[i];
		newBidVol[i] = bidVolumeArray[i];
		newRecTime[i] = recTime[i];
	}

	newPriceArray[insertIndex] = toInsert;
	newRecAskVol[insertIndex] = 0;
	newAskVol[insertIndex] = 0;
	newRecBidVol[insertIndex] = 0;
	newBidVol[insertIndex] = 0;
	newRecTime[insertIndex] = 0;
	priceArrayLength++;

	for (int i = insertIndex + 1; i < priceArrayLength; i++) {
		newPriceArray[i] = priceArray[i - 1];
		newRecAskVol[i] = recAskVolArray[i - 1];
		newAskVol[i] = askVolumeArray[i - 1];
		newRecBidVol[i] = recBidVolArray[i - 1];
		newBidVol[i] = bidVolumeArray[i - 1];
		newRecTime[i] = recTime[i - 1];
	}

	delete priceArray;
	delete recAskVolArray;
	delete askVolumeArray;
	delete recBidVolArray;
	delete bidVolumeArray;
	delete recTime;
	
	priceArray = newPriceArray;
	recAskVolArray = newRecAskVol;
	askVolumeArray = newAskVol;
	recBidVolArray = newRecBidVol;
	bidVolumeArray = newBidVol;
	recTime = newRecTime;

	return insertIndex;
}