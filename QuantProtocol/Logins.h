#pragma once
#include "ImplRApi.h"

int checkAgreements(tsNCharcb connectPoint);

int loginRepo(tsNCharcb connectPoint);
int logoutRepo();

int login(tsNCharcb* ihConnectPoint, tsNCharcb* mdConnectPoint, tsNCharcb* tsConnectPoint, tsNCharcb* plConnectPoint);
int logout();