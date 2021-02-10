#include "Globals.h"

using namespace std;
using namespace RApi;

globals::globals() {
	pAccounts = NULL;
}
globals::~globals() {
	if (this->pAccounts != NULL) {
		delete this->pAccounts;
	}
}