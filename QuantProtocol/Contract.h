#pragma once
#include "RApiPlus.h"
#include "Globals.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <mutex>
#include <string.h>

#include "Windows.h"

using namespace std;
using namespace RApi;

struct Trade {
	double price = 0;
	double size = 0;
	int time = 0;
	tsNCharcb aggrSide;
};

class OrderBook {
public:
	/*	======================== Declare Member Variables ========================	*/
	double* priceArray;
	double* timeArray;
	int domLength;

	int bestAskIndex;
	int* askOrderArray;
	int* askSizeArray;

	int bestBidIndex;
	int* bidOrderArray;
	int* bidSizeArray;


	/*	======================== Declare Member Functions ========================	*/
	OrderBook();
	~OrderBook();

	int updateBook(LimitOrderBookInfo* tick);
	int updateAsk(AskInfo* tick);
	int updateBid(BidInfo* tick);

};

class OrderFlow {
public:
	/*	======================== Declare Member Variables ========================	*/
	double* priceArray;
	int priceArrayLength;

	int* recAskVolArray;
	int* askVolumeArray;

	int* recBidVolArray;
	int* bidVolumeArray;

	int* recTime;

	vector<Trade> tradeFilter;

	/*	======================== Declare Member Functions ========================	*/
	OrderFlow();
	~OrderFlow();

	int updateTrades(TradeInfo* tick);

//private:
	int findPriceIndex(double toFind);
	int insertPrice(double toInsert);
};

/*
* class OrderBars {
* 
* };
*/

class Contract {
public:
	/*	======================== Declare Member Variables ========================	*/
    REngine* pEngine;
    globals* callbackResponses;

    mutex Dom;
    mutex Tape;

	OrderBook* book;
	OrderFlow* flow;

	tsNCharcb exchange;
	tsNCharcb ticker;

	int position;
	
	/*	======================== Declare Member Functions ========================	*/
	Contract(REngine* toEngine, globals* responses, char* toExchange, char* toTicker);
	Contract(const Contract &orig);
	~Contract();

	int subscribe();
	int unsubscribe();

};