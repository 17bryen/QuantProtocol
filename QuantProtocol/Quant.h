#pragma once

#include "ImplRApi.h"
#include "Analysis.h"
#include "OrderManager.h"

class Quant {
	/*	======================== Declare Member Variables ========================	*/
private:
	REngine* pEngine;
	ImplAdmCallbacks* pAdmCallbacks;
	ImplCallbacks* pCallbacks;

public:
	Systems* pSystem;
	AnalysisManager* analysisManager;
	OrderManager* orderManager;


	/*	======================== Declare Member Functions ========================	*/

	Quant();
	~Quant();

	int init(REngineParams &oParams);
	int initThreads();

	bool setAnalyManager(AnalysisManager* toAnaly);
	bool setOrdManager(OrderManager* toOrder);
};


