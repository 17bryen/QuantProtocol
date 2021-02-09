#pragma once
#include "RApiPlus.h"
#include "Globals.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <mutex>

class Account {
public:
	/*	======================== Declare Member Variables ========================	*/
	tsNCharcb fcmId;
	tsNCharcb ibId;
	tsNCharcb accName;

	int accId;
	double accBalance;

	AccountInfo* acc;
	REngine* pEngine;
	globals* responseCallbacks;

	/*	======================== Declare Member Functions ========================	*/
	Account();
	Account(REngine* engine, globals* responses);
	Account(int Id, tsNCharcb* name, tsNCharcb* fcm, tsNCharcb* ib);
	~Account();

	int initAcc();
	int subscribe();
	int unsubscribe();
};