#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <chrono>
#include "RApiPlus.h"

using namespace std;
using namespace RApi;

struct globals {
	globals();
	~globals();

	AccountListInfo* pAccounts;

	bool bRcvdUnacceptedAgreements = false;
    bool bRcvdLimitOrderBook = false;
	bool bRcvdReplayTrades = false;
	bool bRcvdAccountsList = false;
	bool bRcvdTradeRoutes = false;
	bool bLoggedIn = false;
	bool bRcvdPnl = false;

	int iSelectedAccount = 0;
	int iUnacceptedMandatoryAgreements = 0;
	int iRepLoginStatus = 0;
	int iMdLoginStatus = 0;
	int iTsLoginStatus = 0;
	int iIhLoginStatus = 0;
	int iPnlLoginStatus = 0;
};

int getCurrentTime();