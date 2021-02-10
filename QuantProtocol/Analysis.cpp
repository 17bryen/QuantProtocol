#include "Analysis.h"

using namespace std;
using namespace RApi;

int Analysis(Quant* Q) {
	cout << "Begining analysis with account " << Q->accounts.at(0).accName.pData << " with balance $" << Q->accounts.at(0).accBalance << endl;
	while (Q->runtime) {
		cout << endl << "bid: " << Q->watchList.at(0).book->priceArray[Q->watchList.at(0).book->bestBidIndex]
			<< "  ask: " << Q->watchList.at(0).book->priceArray[Q->watchList.at(0).book->bestAskIndex] << endl;
		Sleep(5000);
	}
	cout << endl << "Ending trade analysis..." << endl;
	return 0;
}

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