#include "Strategy.h"

using namespace std;
using namespace RApi;

class LimitOrderVolume {
private:
	int averageVolume;

	vector<zone*>* toSupp;
	vector<zone*>* toResist;
	OrderBook* book;
public:
	LimitOrderVolume(OrderBook* toBook, vector<zone*>* support, vector<zone*>* resists) {
		book = toBook;
		toSupp = support;
		toResist = resists;

		averageVolume = 0;
	}
	~LimitOrderVolume() {

	}
	int analysis(double* bidAsk) {
		averageVolume = 0;

		for (int i = (book->bestBidIndex - 200); i < (book->bestAskIndex + 200); i++) {
			averageVolume += book->askSizeArray[i];
			averageVolume += book->bidSizeArray[i];
		}

		averageVolume /= ((book->bestAskIndex + 200) - (book->bestBidIndex - 200));
		cout << endl << "Average Limit Order Volume: " << averageVolume << endl;

		int temp;
		for (int i = book->bestBidIndex - 200; i < book->bestAskIndex + 200; i++) {
			temp = (book->askSizeArray[i - 1] + book->askSizeArray[i] + book->askSizeArray[i + 1]) / 3;
			if (temp > (2 * averageVolume)) {
				if (i <= book->bestBidIndex) {

					/*	------------------------- Delete Redundancies ----------------------------	*/

					toSupp->push_back(new zone(book->priceArray[i], getCurrentTime()));
					for (int j = 0; j < toSupp->size() - 1; j++) {
						if (toSupp->at(j)->price == toSupp->at(toSupp->size() - 1)->price) {
							toSupp->pop_back();
							break;
						}
					}

				}
				else if (i >= book->bestAskIndex) {

					/*	------------------------- Delete Redundancies ----------------------------	*/

					toResist->push_back(new zone(book->priceArray[i], getCurrentTime()));
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