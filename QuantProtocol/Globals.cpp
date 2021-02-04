#include "Globals.h"

using namespace std;
using namespace RApi;

globals::globals() {
	pAccounts = NULL;
	pSelectedAccountPnl = NULL;
}
globals::~globals() {
	if (this->pAccounts != NULL) {
		delete this->pAccounts;
	}
	if (this->pSelectedAccountPnl != NULL) {
		delete this->pSelectedAccountPnl;
	}
}