#pragma once
#include "ImplRApi.h"

const int LoginStatus_NotLoggedIn = 0;
const int LoginStatus_AwaitingResults = 1;
const int LoginStatus_Failed = 2;
const int LoginStatus_Complete = 3;

struct globals {
	globals();
	~globals();
	REngine* pEngine;
	ImplAdmCallbacks* pAdmCallbacks;
	ImplCallbacks* pCallbacks;

	AccountListInfo* pAccounts;
	PnlInfo* pSelectedAccountPnl;

	bool bRcvdUnacceptedAgreements = false;
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

extern globals *g;
