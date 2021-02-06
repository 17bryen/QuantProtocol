#include "Contract.h"

using namespace std;
using namespace RApi;


Contract::Contract(char* toExchange, char* toTicker) {
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
int Contract::init() {

}


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
int OrderBook::init(tsNCharcb* toExchange, tsNCharcb* toTicker) {

}


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
int OrderFlow::init(tsNCharcb* toExchange, tsNCharcb* toTicker) {

}