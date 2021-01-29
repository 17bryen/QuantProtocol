#pragma once
#include "ImplRApi.h"

int checkAgreements(tsNCharcb connectPoint);

int loginRepo(tsNCharcb connectPoint);
int logoutRepo();

int login(tsNCharcb mdConnectPoint, tsNCharcb tsConnectPoint);
int logout();