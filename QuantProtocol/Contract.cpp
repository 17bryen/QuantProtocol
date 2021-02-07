#include "Contract.h"

using namespace std;
using namespace RApi;

/*   =====================================================================   */
/*                          Contract Definitions                             */
/*   =====================================================================   */

Contract::Contract(REngine* toEngine, globals* responses, char* toExchange, char* toTicker) {
    pEngine = toEngine;
    callbackResponses = responses;

	exchange.pData = toExchange;
	exchange.iDataLen = (int)strlen(exchange.pData);

	ticker.pData = toTicker;
	ticker.iDataLen = (int)strlen(ticker.pData);

	book = new OrderBook();
	flow = new OrderFlow();
}

Contract::~Contract() {
	delete book;
	delete flow;
}

/*   =====================================================================   */

int Contract::subscribe() {
    int iCode;
	int iFlags = (MD_QUOTES | MD_PRINTS);

	if (!pEngine->subscribe(&exchange, &ticker, iFlags, &iCode)) {
		cout << "REngine::subscribe() error : " << iCode << endl;

		return (1);
	}

    //Request Book image

    //Wait for Book image received before continuing
	while (!callbackResponses->bRcvdLimitOrderBook)
		Sleep(1000);

    return 0;
}

int Contract::unsubscribe() {
    int iCode;
    if (!pEngine->unsubscribe(&exchange, &ticker, &iCode)) {
		cout << "REngine::unsubscribe() error : " << iCode << endl;
		return 1;
	}

    return 0;
}

/*   =====================================================================   */
/*                          OrderBook definitions                            */
/*   =====================================================================   */


OrderBook::OrderBook() {
	askPriceArray = nullptr;
	askOrdersArray = nullptr;
	askSizeArray = nullptr;
	askArrayLength = 0;

	bidPriceArray = nullptr;
	bidOrdersArray = nullptr;
	bidSizeArray = nullptr;
	bidArrayLength = 0;
}
OrderBook::~OrderBook() {

}


/*   =====================================================================   */
/*                          OrderFlow definitions                            */
/*   =====================================================================   */


OrderFlow::OrderFlow() {
	askPriceArray = nullptr;
	askVolumeArray = nullptr;
	askArrayLength = 0;

	bidPriceArray = nullptr;
	bidVolumeArray = nullptr;
	bidArrayLength = 0;
}
OrderFlow::~OrderFlow() {

}

/*   =====================================================================   */

