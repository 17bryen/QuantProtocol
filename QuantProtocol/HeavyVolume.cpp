#include "Analysis.h"

class HeavyVolume {
private:
	//double* priceArray;
	//bool* choiceArray;

	int averageVolume;
	//int arrayLength;

	vector<double> toReturn;
	OrderFlow* flow;
public:
	HeavyVolume(OrderFlow* toFlow) {
		flow = toFlow;
		toReturn = {};
		toReturn.reserve(25);

		//priceArray = {};
		//choiceArray = {};

		averageVolume = 0;
		//arrayLength = 0;
	}
	~HeavyVolume() {
		//delete priceArray;
		//delete choiceArray;
	}
	vector<double>* analysis() {
		//If returning a vector anyway, what use do the other two arrays have?
		toReturn.clear();

		/*
		if (arrayLength != flow->priceArrayLength) {
			double* newPrice = new double[flow->priceArrayLength];
			bool* newChoice = new bool[flow->priceArrayLength];
			memcpy(newPrice, flow->priceArray, flow->priceArrayLength);

			for (int i = 0; i < arrayLength; i++) {
				if (choiceArray[i]) {
					newChoice[flow->findPriceIndex(priceArray[i])] = true;
				}
			}
			
			arrayLength = flow->priceArrayLength;
			delete priceArray;
			delete choiceArray;
			priceArray = newPrice;
			choiceArray = newChoice;
		}
		*/
		averageVolume = 0;

		for (int i = 0; i < flow->priceArrayLength; i++) {
			averageVolume += flow->recAskVolArray[i];
			averageVolume += flow->recBidVolArray[i];
		}

		averageVolume /= flow->priceArrayLength;
		cout << endl << "Average Volume: " << averageVolume << endl;
		for (int i = 0; i < flow->priceArrayLength; i++)
			if ((flow->recAskVolArray[i] + flow->recBidVolArray[i]) > (int)(1.6 * averageVolume))
				toReturn.push_back(flow->priceArray[i]);

		return &toReturn;
	}
};