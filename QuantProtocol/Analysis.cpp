#include "Analysis.h"

/**/
#include "HeavyVolume.cpp"
/**/
#include "LimitOrderVolume.cpp"

using namespace std;
using namespace RApi;

int Analysis(Quant* Q) {
	cout << "Begining analysis with account " << Q->accounts.at(0).accName.pData << " with balance $" << Q->accounts.at(0).accBalance << endl;

	vector<double>* SRLevels = new vector<double>(25);
	vector<double>* temp = {};
	HeavyVolume* heavyVol = new HeavyVolume(Q->watchList.at(0).flow);
	LimitOrderVolume* limitVol = new LimitOrderVolume(Q->watchList.at(0).book);

	while (Q->runtime) {
		/*	------------------------- Order Flow Analysis ----------------------------	*/
		Q->watchList.at(0).Tape.lock();
		temp = heavyVol->analysis();
		
		Q->watchList.at(0).Tape.unlock();

		for (int i = 0; i < temp->size(); i++) {
			if (temp->at(i) == 0)
				continue;
			SRLevels->push_back(temp->at(i));
			for (int j = 0; j < SRLevels->size() - 1; j++)
				if (SRLevels->at(j) == SRLevels->at(SRLevels->size() - 1)) {
					SRLevels->pop_back();
					break;
				}
		}
		/*	------------------------- Order Book Analysis ----------------------------	*/
		Q->watchList.at(0).Dom.lock();
		temp = limitVol->analysis();
		
		Q->watchList.at(0).Dom.unlock();

		for (int i = 0; i < temp->size(); i++) {
			if (temp->at(i) == 0)
				continue;
			SRLevels->push_back(temp->at(i));
			for (int j = 0; j < SRLevels->size() - 1; j++)
				if (SRLevels->at(j) == SRLevels->at(SRLevels->size() - 1)) {
					SRLevels->pop_back();
					break;
				}
		}
		/*	------------------------- Delete Redundancies ----------------------------	*/
		

		/*	------------------------ Confirmation Analysis ---------------------------	*/


		//Output
		cout << endl << "The following prices have been identified as supports and/or resistances: " << endl;
		for (int i = 0; i < SRLevels->size(); i++)
			cout << SRLevels->at(i) << endl;
		Sleep(3000);
	}
	cout << endl << "Ending trade analysis..." << endl;

	delete SRLevels;
	delete heavyVol;
	return 0;
};

/*CODE TO PRINT DOM OVER 20 SECONDS:
 	for (int i = 0; i < 10; i++) {
		Q->watchList.at(0).Dom.lock();
		cout << endl << endl;
		cout << "  bid  | price |  ask  " << endl;

		for (int j = Q->watchList.at(0).book->bestAskIndex + 5; j >= Q->watchList.at(0).book->bestBidIndex - 5; j--) {
			cout <<
				Q->watchList.at(0).book->bidSizeArray[j] << "  " <<
				Q->watchList.at(0).book->priceArray[j] << "  " <<
				Q->watchList.at(0).book->askSizeArray[j] << endl;
		}
			
		Q->watchList.at(0).Dom.unlock();

		Sleep(2000);
	}
*/

/*
cout << endl << "bid: " << Q->watchList.at(0).book->priceArray[Q->watchList.at(0).book->bestBidIndex]
	<< "  ask: " << Q->watchList.at(0).book->priceArray[Q->watchList.at(0).book->bestAskIndex] << endl;
*/