#pragma once

#include "Globals.h"
#include "Contract.h"
#include "ImplRApi.h"
#include "Quant.h"

struct zone {
	double price;
	bool pulledAway;
	bool confirmed;
	int timeObserved;

	zone(double p, int time) {
		price = p;
		timeObserved = time;
		pulledAway = false;
		confirmed = false;
	}
	~zone() {}
};


int Analysis(Quant* Q);