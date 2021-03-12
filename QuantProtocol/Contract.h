#pragma once

#include "RApiPlus.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>

#include "Windows.h"

using namespace std;
using namespace RApi;

int getCurrentTime();

struct Trade {
	double price = 0;
	double size = 0;
	int time = 0;
	char aggrSide = 'A';
};

struct pendingOrder {
	char side;
	char purpose;
	bool submitted = false;

	pendingOrder(char toSide, char toPurpose) {
		side = toSide;
		purpose = toPurpose;
	}
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
	int* recTime;
	int priceArrayLength;

	int* recAskVolArray;
	int* askVolumeArray;

	int* recBidVolArray;
	int* bidVolumeArray;

	vector<Trade*>* tradeFilter;

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
private:
	REngine* pEngine;

public:
	/*	======================== Declare Member Variables ========================	*/

    mutex domLock;
    mutex tapeLock;

	OrderBook* book;
	OrderFlow* flow;
	pendingOrder* pending;

	tsNCharcb exchange;
	tsNCharcb ticker;

	int positionSize;
	bool rcvdReplayTrades;
	bool rcvdLimitOrderBook;
	
	/*	======================== Declare Member Functions ========================	*/
	Contract(REngine* toEngine, char* toExchange, char* toTicker);
	Contract(const Contract &orig);
	~Contract();

	int subscribe();
	int unsubscribe();

};