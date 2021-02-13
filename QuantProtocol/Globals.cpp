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

int getCurrentTime() {
	int toReturn = chrono::duration_cast<chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count();
	//DONT? ACCOUNT FOR TIME DIFFERENCE (times in GMT; KW 3 hours ahead GMT; KW 9 ahead CST) (Already returns UTC which = GMT)
	//toReturn -= (60 * 60 * 9);

	return toReturn;
}