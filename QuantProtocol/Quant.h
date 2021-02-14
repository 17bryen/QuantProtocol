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
	Systems* system;
	AnalysisManager* analysisManager;
	OrderManager* orderManager;


	/*	======================== Declare Member Functions ========================	*/

	Quant();
	~Quant();

	int init(REngineParams &oParams);

	bool setAnalyManager(AnalysisManager* toAnaly);
	bool setOrdManager(OrderManager* toOrder);
};


