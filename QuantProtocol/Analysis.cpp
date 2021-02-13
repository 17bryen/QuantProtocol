#include "Analysis.h"

/**/
#include "HeavyVolume.cpp"
#include "TradeFilter.cpp"
/**/
#include "LimitOrderVolume.cpp"
#include "Delta.cpp"

using namespace RApi;
using namespace std;

int Analysis(Quant* Q) {
	cout << "Begining analysis with account " << Q->accounts.at(0).accName.pData << " with balance $" << Q->accounts.at(0).accBalance << endl;

	vector<zone>* suppLevels = new vector<zone>;
	vector<zone>* resistLevels = new vector<zone>;
	double* currBidAsk = new double[2];

	HeavyVolume* heavyVol = new HeavyVolume(Q->watchList.at(0).flow, suppLevels, resistLevels);
	LimitOrderVolume* limitVol = new LimitOrderVolume(Q->watchList.at(0).book, suppLevels, resistLevels);
	TradeFilter* tradeFilt = new TradeFilter(Q->watchList.at(0).flow, suppLevels, resistLevels);
	Delta* delta = new Delta(Q->watchList.at(0).book, suppLevels, resistLevels);

	while (Q->runtime) {

		/*	------------------------- Order Book Analysis ----------------------------	*/
		Q->watchList.at(0).Dom.lock();
		currBidAsk[0] = Q->watchList.at(0).book->priceArray[Q->watchList.at(0).book->bestBidIndex];
		currBidAsk[1] = Q->watchList.at(0).book->priceArray[Q->watchList.at(0).book->bestAskIndex];

		//Analysis
		limitVol->analysis(currBidAsk[0], currBidAsk[1]);
		
		//Confirmations
		delta->confirmation(currBidAsk[0], currBidAsk[1]);

		Q->watchList.at(0).Dom.unlock();

		/*	------------------------- Order Flow Analysis ----------------------------	*/
		Q->watchList.at(0).Tape.lock();
		//Analysis
		heavyVol->analysis(currBidAsk[0], currBidAsk[1]);
		tradeFilt->analysis(currBidAsk[0], currBidAsk[1]);

		//Confirmations
		
		Q->watchList.at(0).Tape.unlock();		

		/*	------------------------ Confirmation Analysis ---------------------------	*/

		for (int i = 0; i < suppLevels->size(); i++) {		//Delete support levels the price has broken through
			if ((suppLevels->at(i).price - currBidAsk[0]) >= 1.25)
				suppLevels->erase(suppLevels->begin() + i);

			else if ((currBidAsk[0] - suppLevels->at(i).price) >= 1.5)
				suppLevels->at(i).pulledAway = true;

			else if (suppLevels->at(i).confirmed) {
				Q->accounts.at(0).order->shortMarket(&Q->watchList.at(0));
				suppLevels->erase(suppLevels->begin() + i);
				i--;
			}
		}

		for (int i = 0; i < resistLevels->size(); i++) {	//Delete resistance levels the price has broken through
			if ((currBidAsk[1] - resistLevels->at(i).price) >= 1.25)
				resistLevels->erase(resistLevels->begin() + i);

			else if ((resistLevels->at(i).price - currBidAsk[1]) >= 1.5)
				suppLevels->at(i).pulledAway = true;

			else if (resistLevels->at(i).confirmed) {
				Q->accounts.at(0).order->shortMarket(&Q->watchList.at(0));
				resistLevels->erase(resistLevels->begin() + i);
				i--;
			}
		}

		/*	------------------------ Final Analysis Output ---------------------------	*/

		cout << endl << "The current bid/ask spread is " << currBidAsk[0] << " / " << currBidAsk[1];
		cout << endl << "The following prices have been identified as possible supports: " << endl;
		for (int i = 0; i < suppLevels->size(); i++)
			cout << suppLevels->at(i).price << endl;
		cout << endl << "The following prices have been identified as possible resistances: " << endl;
		for (int i = 0; i < resistLevels->size(); i++)
			cout << resistLevels->at(i).price << endl;
		Sleep(500);

	}

	cout << endl << "Ending trade analysis..." << endl;

	delete heavyVol;
	delete limitVol;
	return 0;
};

