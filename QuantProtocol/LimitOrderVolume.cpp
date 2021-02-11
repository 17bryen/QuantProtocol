#include "Analysis.h"

using namespace std;
using namespace RApi;

class LimitOrderVolume {
private:
	int averageVolume;

	vector<double> toReturn;
	OrderBook* book;
public:
	LimitOrderVolume(OrderBook* toBook) {
		book = toBook;
		toReturn = {};
		toReturn.reserve(50);

		averageVolume = 0;
	}
	~LimitOrderVolume() {

	}
	vector<double>* analysis() {
		toReturn.clear();

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
			if (temp > (1.6 * averageVolume))
				toReturn.push_back(book->priceArray[i]);
		}

		return &toReturn;
	}
};