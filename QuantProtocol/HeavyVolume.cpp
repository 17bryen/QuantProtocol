#include "Strategy.h"

class HeavyVolume {
private:
	int averageVolume;

	vector<zone*>* toSupp;
	vector<zone*>* toResist;
	OrderFlow* flow;
public:
	HeavyVolume(OrderFlow* toFlow, vector<zone*>* support, vector<zone*>* resists) {
		flow = toFlow;
		toSupp = support;
		toResist = resists;

		averageVolume = 0;
	}
	~HeavyVolume() {
	}

	int analysis(double* bidAsk) {
		averageVolume = 0;

		for (int i = 0; i < flow->priceArrayLength; i++) {
			averageVolume += flow->recAskVolArray[i];
			averageVolume += flow->recBidVolArray[i];
		}

		averageVolume /= flow->priceArrayLength;
		cout << endl << "Average Volume: " << averageVolume << endl;
		for (int i = 0; i < flow->priceArrayLength; i++) {
			if ((flow->recAskVolArray[i] + flow->recBidVolArray[i]) > (int)(2 * averageVolume)) {

				/*	------------------------- Delete Redundancies ----------------------------	*/
				if (!(flow->recBidVolArray[i] - flow->recAskVolArray[i] > (3 * averageVolume / 4))) {
					toSupp->push_back(new zone(flow->priceArray[i], flow->recTime[i]));
					for (int j = 0; j < toSupp->size() - 1; j++) {
						if (toSupp->at(j)->price == toSupp->at(toSupp->size() - 1)->price) {
							toSupp->pop_back();
							break;
						}
					}
				}

				/*	------------------------- Delete Redundancies ----------------------------	*/
				if (!(flow->recAskVolArray[i] - flow->recBidVolArray[i] > (3 * averageVolume / 4))) {
					toResist->push_back(new zone(flow->priceArray[i], flow->recTime[i]));
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
};