#pragma once
#include "RApiPlus.h"
#include "Globals.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <mutex>

#include "Windows.h"

using namespace std;
using namespace RApi;

class OrderBook {
public:
	/*	======================== Declare Member Variables ========================	*/
	double* priceArray;
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
	double* askPriceArray;
	int* askVolumeArray;
	int askArrayLength;

	double* bidPriceArray;
	int* bidVolumeArray;
	int bidArrayLength;


	/*	======================== Declare Member Functions ========================	*/
	OrderFlow();
	~OrderFlow();

	int updateTrades(TradeInfo* tick);

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


	/*	======================== Declare Member Functions ========================	*/
	Contract(REngine* toEngine, globals* responses, char* toExchange, char* toTicker);
	Contract(const Contract &orig);
	~Contract();

	int subscribe();
	int unsubscribe();

};