#pragma once

#include "Systems.h"
#include "Strategy.h"

/**/
#include "HeavyVolume.cpp"
#include "TradeFilter.cpp"
/**/
#include "LimitOrderVolume.cpp"
#include "Delta.cpp"

struct analysis {
	vector<zone*>* suppLevels;
	vector<zone*>* resistLevels;

	OrderBook* book;
	OrderFlow* flow;

	double* currBidAsk;
	bool toEnter;

	analysis(OrderBook* toBook, OrderFlow* toFlow);
	~analysis();

	HeavyVolume* heavyVol;
	TradeFilter* tradeFilt;

	LimitOrderVolume* limitVol;
	Delta* delta;
};

class AnalysisManager {
private:
	REngine* pEngine;
	Systems* pSystem;

	vector<Contract*>* watchlist;
	vector<analysis*>* watchlistAnalysis;

public:
	AnalysisManager(REngine* toEngine, Systems* toSys);
	~AnalysisManager();

	int Analysis();
};
