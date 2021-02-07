#pragma once
#include "RApiPlus.h"
#include "Globals.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "Windows.h"

using namespace std;
using namespace RApi;

class OrderBook {
public:
	/*	======================== Declare Member Variables ========================	*/
	double* askPriceArray;
	int* askOrdersArray;
	int* askSizeArray;
	int askArrayLength;

	double* bidPriceArray;
	int* bidOrdersArray;
	int* bidSizeArray;
	int bidArrayLength;


	/*	======================== Declare Member Functions ========================	*/
	OrderBook();
	~OrderBook();

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

	tsNCharcb exchange;
	tsNCharcb ticker;

	OrderBook* book;
	OrderFlow* flow;


	/*	======================== Declare Member Functions ========================	*/
	Contract(REngine* toEngine, globals* responses, char* toExchange, char* toTicker);
	~Contract();

	int subscribe();
	int unsubscribe();

};