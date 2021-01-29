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

	bool bRcvdUnacceptedAgreements = false;

	int iUnacceptedMandatoryAgreements = 0;
	int iRepLoginStatus = 0;
	int iMdLoginStatus = 0;
	int iTsLoginStatus = 0;
};

extern globals *g;
