#include "Analysis.h"

using namespace std;
using namespace RApi;

class Delta {
private:
	int nearBidSize;
	int nearAskSize;
	double delta;

	vector<zone>* toSupp;
	vector<zone>* toResist;
	OrderBook* book;
public:
	Delta(OrderBook* toBook, vector<zone>* support, vector<zone>* resists) {
		book = toBook;
		toSupp = support;
		toResist = resists;

		nearBidSize = 0;
		nearAskSize = 0;
		delta = 0;
	}
	~Delta() { };
	int confirmation(double bid, double ask) {
		nearBidSize = 0;
		nearAskSize = 0;
		delta = 0;

		for (int i = 0; i < 4; i++) {
			nearBidSize += book->bidSizeArray[book->bestBidIndex - i];
			nearAskSize += book->askSizeArray[book->bestAskIndex + i];
		}

		nearBidSize /= 4;
		nearAskSize /= 4;
		delta = nearBidSize / (nearBidSize + nearAskSize);
		cout << endl << "Current OrderBook Delta: " << delta << endl;

		if (delta >= 0.53) {	//Large bid - buy
			for (int i = 0; i < toSupp->size(); i++) {
				if (!toSupp->at(i).confirmed && toSupp->at(1).pulledAway
					&& (toSupp->at(1).price - bid <= 1 && toSupp->at(1).price - bid >= -1))

					toSupp->at(i).confirmed = true;
			}
		} 
		else if (delta <= 0.47) {	//Large ask - sell
			for (int i = 0; i < toResist->size(); i++) {
				if (!toResist->at(i).confirmed && toResist->at(1).pulledAway
					&& (toResist->at(1).price - ask <= 1 && toResist->at(1).price - ask >= -1))

					toResist->at(i).confirmed = true;
			}
		}
		return 0;
	}
};