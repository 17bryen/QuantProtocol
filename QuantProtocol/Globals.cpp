#include "Globals.h"

globals::globals() {
	pAdmCallbacks = NULL;
	pCallbacks = NULL;
	pEngine = NULL;
}
globals::~globals() {
	if (this->pAdmCallbacks) {
		delete this->pAdmCallbacks;
		if (this->pCallbacks) {
			delete this->pCallbacks;
			if (this->pEngine)
				delete this->pEngine;
		}
	}
}