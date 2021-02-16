#pragma once

#include "Systems.h"

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