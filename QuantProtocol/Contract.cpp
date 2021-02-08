#include "Contract.h"


using namespace std;
using namespace RApi;

/*   =====================================================================   */
/*                          Contract Definitions                             */
/*   =====================================================================   */

Contract::Contract(REngine* toEngine, globals* responses, char* toExchange, char* toTicker) {
    pEngine = toEngine;
    callbackResponses = responses;

	exchange.pData = toExchange;
	exchange.iDataLen = (int)strlen(exchange.pData);

	ticker.pData = toTicker;
	ticker.iDataLen = (int)strlen(ticker.pData);

	book = new OrderBook();
	flow = new OrderFlow();
}

Contract::Contract(const Contract &orig) {
	Dom.lock();

	pEngine = orig.pEngine;
	callbackResponses = orig.callbackResponses;

	book = orig.book;
	flow = orig.flow;

	exchange.pData = orig.exchange.pData;
	exchange.iDataLen = orig.exchange.iDataLen;

	ticker.pData = orig.ticker.pData;
	ticker.iDataLen = orig.ticker.iDataLen;

	Dom.unlock();
}

Contract::~Contract() {
	delete book;
	delete flow;
}

/*   =====================================================================   */

int Contract::subscribe() {
    int iCode;
	int iFlags = (MD_QUOTES | MD_PRINTS);

	if (!pEngine->subscribe(&exchange, &ticker, iFlags, &iCode)) {
		cout << "REngine::subscribe() error : " << iCode << endl;

		return (1);
	}

    //Wait for Book image received before continuing
	while (!callbackResponses->bRcvdLimitOrderBook)
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

//OrderBook ask and bid arrays start[0] at the spread
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
		cout << endl << "could not find ask array price update!" << endl;
		return 1;
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
		cout << endl << "could not find bid array price update!" << endl;
		return 1;
	}

	return 0;
}

/*   =====================================================================   */
/*                          OrderFlow definitions                            */
/*   =====================================================================   */


OrderFlow::OrderFlow() {
	askPriceArray = nullptr;
	askVolumeArray = nullptr;
	askArrayLength = 0;

	bidPriceArray = nullptr;
	bidVolumeArray = nullptr;
	bidArrayLength = 0;
}
OrderFlow::~OrderFlow() {
	delete askPriceArray;
	delete askVolumeArray;
	
	delete bidPriceArray;
	delete bidVolumeArray;
}

/*   =====================================================================   */

int OrderFlow::updateTrades(TradeInfo* tick) {
	cout << endl << tick->iSize << " @" << tick->dPrice;
	return 0;
}