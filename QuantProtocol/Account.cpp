#include "Account.h"

Account::Account() {
	fcmId.pData = NULL;
	fcmId.iDataLen = 0;
	ibId.pData = NULL;
	ibId.iDataLen = 0;
	accName.pData = NULL;
	accName.iDataLen = 0;

	accId.pData = NULL;
	accId.iDataLen = 0;

	accBalance = 0;

	account = nullptr;
	pEngine = nullptr;
	responseCallbacks = nullptr;
}
Account::Account(AccountInfo* acc) {
	fcmId.pData = acc->sFcmId.pData;
	fcmId.iDataLen = acc->sFcmId.iDataLen;
	ibId.pData = acc->sIbId.pData;
	ibId.iDataLen = acc->sIbId.iDataLen;
	accName.pData = acc->sAccountName.pData;
	accName.iDataLen = acc->sAccountName.iDataLen;

	accId.pData = acc->sAccountId.pData;
	accId.iDataLen = acc->sAccountId.iDataLen;

	accBalance = 0;

	account = acc;
	pEngine = nullptr;
	responseCallbacks = nullptr;
}
Account::Account(REngine* engine, globals* responses) {
	fcmId.pData = NULL;
	fcmId.iDataLen = 0;
	ibId.pData = NULL;
	ibId.iDataLen = 0;
	accName.pData = NULL;
	accName.iDataLen = 0;

	accId.pData = NULL;
	accId.iDataLen = 0;

	accBalance = 0;

	account = nullptr;
	pEngine = engine;
	responseCallbacks = responses;
}
Account::Account(REngine* engine, globals* responses, AccountInfo* acc) {
	fcmId.pData = acc->sFcmId.pData;
	fcmId.iDataLen = acc->sFcmId.iDataLen;
	ibId.pData = acc->sIbId.pData;
	ibId.iDataLen = acc->sIbId.iDataLen;
	accName.pData = acc->sAccountName.pData;
	accName.iDataLen = acc->sAccountName.iDataLen;

	accId.pData = acc->sAccountId.pData;
	accId.iDataLen = acc->sAccountId.iDataLen;

	accBalance = 0;

	account = acc;
	pEngine = engine;
	responseCallbacks = responses;
}
Account::~Account() {
	
}

/*	=========================================================================	*/

int Account::initAcc() {

	return 0;
}
int Account::subscribe() {
	int iCode;
	try {
		pEngine->subscribePnl(account, &iCode);
	}
	catch (OmneException &oEx){
		cout << endl << "Failed to subscribe to PnL server for account ID " << this->accName.pData
			<< ";  ERRORS: " << iCode << endl;
		return 1;
	}
	return 0;
}
int Account::unsubscribe() {
	int iCode;
	pEngine->unsubscribePnl(account, &iCode);
	return 1;
}