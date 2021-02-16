#include "Strategy.h"

using namespace std;
using namespace RApi;

class TradeFilter {
private:
	int averageOrder;

	vector<zone*>* toSupp;
	vector<zone*>* toResist;
	OrderFlow* flow;
public:
	TradeFilter(OrderFlow* toFlow, vector<zone*>* support, vector<zone*>* resists) {
		flow = toFlow;
		toSupp = support;
		toResist = resists;

		averageOrder = 0;
	}
	~TradeFilter() {}

	int analysis(double* bidAsk) {
		averageOrder = 0;

		if (flow->tradeFilter->size() != 0) {
			for (int i = 0; i < flow->tradeFilter->size(); i++) {
				averageOrder += flow->tradeFilter->at(i)->size;
			}
			averageOrder /= flow->tradeFilter->size();
		}
		else
			averageOrder = 20;

		for (int i = 0; i < flow->tradeFilter->size(); i++) {
			if (flow->tradeFilter->at(i)->size > (averageOrder * 2)) {

				if (flow->tradeFilter->at(i)->aggrSide == 'B') {
					toSupp->push_back(new zone(flow->tradeFilter->at(i)->price, flow->tradeFilter->at(i)->time));
					for (int j = 0; j < toSupp->size() - 1; j++) {
						if (toSupp->at(j)->price == toSupp->at(toSupp->size() - 1)->price) {
							toSupp->pop_back();
							break;
						}
					}
				}
				else if (flow->tradeFilter->at(i)->aggrSide == 'S') {
					toResist->push_back(new zone(flow->tradeFilter->at(i)->price, flow->tradeFilter->at(i)->time));
					for (int j = 0; j < toResist->size() - 1; j++) {
						if (toResist->at(j)->price == toResist->at(toResist->size() - 1)->price) {
							toResist->pop_back();
							break;
						}
					}
				}
			}
		}

		return 0;
	}
	int confirmation(double* bidAsk) {
		for (int i = 0; i < toSupp->size(); i++) {
			if (!toSupp->at(i)->confirmed && toSupp->at(i)->pulledAway && ((toSupp->at(i)->price - bidAsk[0]) * (toSupp->at(i)->price - bidAsk[0] ) < 1))
				for (int j = 0; j < flow->tradeFilter->size(); j++) {
					//if (toSupp->at(i).price - flow->tradeFilter
				}
		}

		for (int i = 0; i < toSupp->size(); i++) {
			if (!toSupp->at(i)->confirmed && toSupp->at(i)->pulledAway && ((toSupp->at(i)->price - bidAsk[0]) * (toSupp->at(i)->price - bidAsk[0] ) < 1))
				for (int j = 0; j < flow->tradeFilter->size(); j++) {
					//if (toSupp->at(i).price - flow->tradeFilter
				}
		}
	}
};