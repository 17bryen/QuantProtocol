#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "RApiPlus.h"

using namespace std;
using namespace RApi;

struct globals {
	globals();
	~globals();

	AccountListInfo* pAccounts;
	PnlInfo* pSelectedAccountPnl;

	bool bRcvdUnacceptedAgreements = false;
    bool bRcvdLimitOrderBook = false;
	bool bRcvdReplayTrades = false;
	bool bRcvdAccountsList = false;
	bool bRcvdExchanges = false;
	bool bRcvdPnl = false;

	int iSelectedAccount = 0;
	int iUnacceptedMandatoryAgreements = 0;
	int iRepLoginStatus = 0;
	int iMdLoginStatus = 0;
	int iTsLoginStatus = 0;
	int iIhLoginStatus = 0;
	int iPnlLoginStatus = 0;
};