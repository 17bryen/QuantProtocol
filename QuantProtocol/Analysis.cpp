#include "Analysis.h"

using namespace RApi;
using namespace std;


/*	======================================================================================	*/
/*										Define Structures									*/	
/*	======================================================================================	*/

AnalysisManager::AnalysisManager(REngine* toEngine, Systems* toSys) {
	pEngine = toEngine;
	pSystem = toSys;
	watchlist = pSystem->acc->watchlist;

	for (int i = 0; i < watchlist->size(); i++)
		watchlistAnalysis->push_back(new analysis(toSys->acc->watchlist->at(i)->book, toSys->acc->watchlist->at(i)->flow));
}
AnalysisManager::~AnalysisManager() {
	for (int w = 0; w < watchlist->size(); w++) {
		for (int i = 0; i < watchlistAnalysis->at(w)->suppLevels->size(); i++)
			delete watchlistAnalysis->at(w)->suppLevels->at(i);
		for (int i = 0; i < watchlistAnalysis->at(w)->resistLevels->size(); i++)
			delete watchlistAnalysis->at(w)->resistLevels->at(i);

		delete watchlistAnalysis->at(w)->currBidAsk;
	}
}

/*	======================================================================================	*/

analysis::analysis(OrderBook* toBook, OrderFlow* toFlow) {
	suppLevels = new vector<zone*>;
	suppLevels->reserve(3);
	resistLevels = new vector<zone*>;
	resistLevels->reserve(3);

	currBidAsk = new double[2];
	book = toBook;
	flow = toFlow;
	toEnter = true;

	heavyVol = new HeavyVolume(flow, suppLevels, resistLevels);
	tradeFilt = new TradeFilter(flow, suppLevels, resistLevels);

	limitVol = new LimitOrderVolume(book, suppLevels, resistLevels);
	delta = new Delta(book, suppLevels, resistLevels);
}
analysis::~analysis() {
	delete heavyVol;
	delete tradeFilt;

	delete limitVol;
	delete delta;

	delete suppLevels;
	delete resistLevels;
	delete currBidAsk;
}

/*	======================================================================================	*/
/*									Main Analysis Function									*/
/*	======================================================================================	*/

int AnalysisManager::Analysis() {
	cout << "Beginning analysis with account " << pSystem->acc->account->sAccountName.pData << " with balance $" << pSystem->acc->accBalance << endl;

	while (pSystem->userManagement) {
		for (int w = 0; w < watchlist->size(); w++) {

			/*	------------------------- Order Book Analysis ----------------------------	*/
			watchlist->at(w)->domLock.lock();

			//Update current bid/ask
			watchlistAnalysis->at(w)->currBidAsk[0] = watchlist->at(w)->book->priceArray[watchlist->at(w)->book->bestBidIndex];
			watchlistAnalysis->at(w)->currBidAsk[1] = watchlist->at(w)->book->priceArray[watchlist->at(w)->book->bestAskIndex];

			//Analysis
			watchlistAnalysis->at(w)->limitVol->analysis(watchlistAnalysis->at(w)->currBidAsk);

			//Confirmations
			watchlistAnalysis->at(w)->delta->confirmation(watchlistAnalysis->at(w)->currBidAsk);

			watchlist->at(w)->domLock.unlock();

			/*	------------------------- Order Flow Analysis ----------------------------	*/
			watchlist->at(w)->tapeLock.lock();

			//Analysis
			watchlistAnalysis->at(w)->heavyVol->analysis(watchlistAnalysis->at(w)->currBidAsk);
			watchlistAnalysis->at(w)->tradeFilt->analysis(watchlistAnalysis->at(w)->currBidAsk);

			//Confirmations


			watchlist->at(w)->tapeLock.unlock();

			/*	------------------------ Confirmation Analysis ---------------------------	*/

			for (int i = 0; i < watchlistAnalysis->at(w)->suppLevels->size(); i++) {		//Delete support levels the price has broken through
				if ((watchlistAnalysis->at(w)->suppLevels->at(i)->price - watchlistAnalysis->at(w)->currBidAsk[0]) >= 1.25)
					watchlistAnalysis->at(w)->suppLevels->erase(watchlistAnalysis->at(w)->suppLevels->begin() + i);

				else if ((watchlistAnalysis->at(w)->currBidAsk[0] - watchlistAnalysis->at(w)->suppLevels->at(i)->price) >= 1.5)
					watchlistAnalysis->at(w)->suppLevels->at(i)->pulledAway = true;

				/*	Don't manually enter trade here, just let order manager read the confirmation signal (how to decide to remove from list?)
				else if (watchlistAnalysis->at(w)->suppLevels->at(i)->confirmed) {
					Q->accounts.at(0).order->shortMarket(&Q->watchList.at(0));
					suppLevels->erase(suppLevels->begin() + i);
					i--;
				}
			*/
			}

			for (int i = 0; i < watchlistAnalysis->at(w)->resistLevels->size(); i++) {	//Delete resistance levels the price has broken through
				if ((watchlistAnalysis->at(w)->currBidAsk[1] - watchlistAnalysis->at(w)->resistLevels->at(i)->price) >= 1.25)
					watchlistAnalysis->at(w)->resistLevels->erase(watchlistAnalysis->at(w)->resistLevels->begin() + i);

				else if ((watchlistAnalysis->at(w)->resistLevels->at(i)->price - watchlistAnalysis->at(w)->currBidAsk[1]) >= 1.5)
					watchlistAnalysis->at(w)->suppLevels->at(i)->pulledAway = true;

				/* Same reasoning as above ^^^
				else if (resistLevels->at(i).confirmed) {
					Q->accounts.at(0).order->shortMarket(&Q->watchList.at(0));
					resistLevels->erase(resistLevels->begin() + i);
					i--;
				}
				*/
			}

			/*	------------------------ Final Analysis Output ---------------------------	*/

			cout << endl << "The current bid/ask spread is " << watchlistAnalysis->at(w)->currBidAsk[0] << " / " << watchlistAnalysis->at(w)->currBidAsk[1];
			cout << endl << "The following prices have been identified as possible supports: " << endl;
			for (int i = 0; i < watchlistAnalysis->at(w)->suppLevels->size(); i++)
				cout << watchlistAnalysis->at(w)->suppLevels->at(i)->price << endl;
			cout << endl << "The following prices have been identified as possible resistances: " << endl;
			for (int i = 0; i < watchlistAnalysis->at(w)->resistLevels->size(); i++)
				cout << watchlistAnalysis->at(w)->resistLevels->at(i)->price << endl;

			Sleep(500);
		}
	}

	cout << endl << "Ending trade analysis..." << endl;
	
	return 0;
};

