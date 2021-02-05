#pragma once
#include "RApiPlus.h"

using namespace std;
using namespace RApi;

class OrderBook {
	/*	======================== Declare Member Variables ========================	*/
	tsNCharcb exchange;
	tsNCharcb ticker;

	double* askPriceArray;
	int* askOrdersArray;
	int* askSizeArray;
	int askArrayLength;

	double* bidPriceArray;
	int* bidOrdersArray;
	int* bidSizeArray;
	int bidArrayLength;


	/*	======================== Declare Member Functions ========================	*/
	OrderBook(tsNCharcb* toExchange, tsNCharcb* toTicker);
	~OrderBook();

	int init(bool sub);
	int unsub();

	int updateAsk(AskInfo* tick);
	int updateBid(BidInfo* tick);
};