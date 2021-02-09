#include "Account.h"

Account::Account() {
	fcmId.pData = NULL;
	fcmId.iDataLen = 0;
	ibId.pData = NULL;
	ibId.iDataLen = 0;
	accName.pData = NULL;
	accName.iDataLen = 0;

	accId = 0;
	accBalance = 0;

	pEngine = nullptr;
	responseCallbacks = nullptr;
}
Account::Account(REngine* engine, globals* responses) {

}
Account::Account(int Id, tsNCharcb* name, tsNCharcb* fcm, tsNCharcb* ib) {

}
Account::~Account() {

}

/*	=========================================================================	*/

int Account::initAcc() {

}
int Account::subscribe() {
	int iCode;
	pEngine->subscribePnl(acc, &iCode);
}
int Account::unsubscribe() {
	int iCode;
	pEngine->unsubscribePnl(acc, &iCode);
}