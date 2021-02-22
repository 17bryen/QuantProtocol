#include "Quant.h"

using namespace std;
using namespace RApi;

/*==============================================================================*/
/*							Define Member Functions							 	*/
/*==============================================================================*/


/* Constructor for main obj class. Initializes default connect points, however 
*	object pointers are left as NULLS so that their initialization may return proper 
*	error values. 
*/
Quant::Quant() {
	pEngine = nullptr;
	pAdmCallbacks = nullptr;
	pCallbacks = nullptr;

	pSystem = new Systems();
	analysisManager = nullptr;
	orderManager = nullptr;
}

/* Deconstructor to properly allocate memory back from complex objects. */
Quant::~Quant() {
	/*	--------------------- Delete Member Variables ------------------------	*/
	

	/*	---------------------- Delete Member Objects -------------------------	*/
	if (pCallbacks != nullptr)
		delete pCallbacks;
	if (pEngine != nullptr)
		delete pEngine;
	if (pAdmCallbacks != nullptr)
		delete pAdmCallbacks;
	if (pSystem != nullptr)
		delete pSystem;
}

/*	==========================================================================	*/

/* Initializes AdmCallbacks, Callbacks, and REngine objects and pointers
	for quant object. If the REngineParams passed in has a NULL value for
	envp, a default one will be constructed. */
int Quant::init(REngineParams &oParams) {
	int iCode;

/*	------------------------- Create fake_envp if not Provided ---------------	*/
	if (oParams.envp == NULL) {
		char* fake_envp[9];

		fake_envp[0] = (char*)"MML_DMN_SRVR_ADDR=ritpz01001.01.rithmic.com:65000~ritpz01000.01.rithmic.com:65000~ritpz01001.01.rithmic.net:65000~ritpz01000.01.rithmic.net:65000~ritpz01001.01.theomne.net:65000~ritpz01000.01.theomne.net:65000~ritpz01001.01.theomne.com:65000~ritpz01000.01.theomne.com:65000";
		fake_envp[1] = (char*)"MML_DOMAIN_NAME=rithmic_prod_01_dmz_domain";
		fake_envp[2] = (char*)"MML_LIC_SRVR_ADDR=ritpz01000.01.rithmic.com:56000~ritpz01001.01.rithmic.com:56000~ritpz01000.01.rithmic.net:56000~ritpz01001.01.rithmic.net:56000~ritpz01000.01.theomne.net:56000~ritpz01001.01.theomne.net:56000~ritpz01000.01.theomne.com:56000~ritpz01001.01.theomne.com:56000";
		fake_envp[3] = (char*)"MML_LOC_BROK_ADDR=ritpz01000.01.rithmic.com:64100";
		fake_envp[4] = (char*)"MML_LOGGER_ADDR=ritpz01000.01.rithmic.com:45454~ritpz01000.01.rithmic.net:45454~ritpz01000.01.theomne.net:45454~ritpz01000.01.theomne.com:45454";
		fake_envp[5] = (char*)"MML_LOG_TYPE=log_net";
		// Specify the file path to the SSL file here
		fake_envp[6] = (char*)"MML_SSL_CLNT_AUTH_FILE=../QuantProtocol/Rithmic/ssl/rithmic_ssl_cert_auth_params";
		fake_envp[7] = (char*)"USER=17bryen@amp.com";
		fake_envp[8] = NULL;

		oParams.envp = fake_envp;
	}


/*	----------------------- Initialize AdmCallbacks --------------------------	*/
	try {
		pAdmCallbacks = new ImplAdmCallbacks();
		cout << "Created AdmCallbacks object... ";
	}
	catch (OmneException& oEx) {
		iCode = oEx.getErrorCode();
		cout << endl << "ImplAdmCallbacks::ImplAdmCallbacks() error : " << iCode << endl;
		return 1;
	}


/*	---------------------------- Initialize REngine --------------------------	*/
	oParams.pContext = (void*)pSystem;
	oParams.pAdmCallbacks = pAdmCallbacks;
	try {
		pEngine = new REngine(&oParams);
		cout << "Created REngine object... ";
	}
	catch (OmneException& oEx) {
		delete pAdmCallbacks;

		iCode = oEx.getErrorCode();
		cout << "REnging::REngine() error : " << iCode << endl;
		return 2;
	}


/*	-------------------------- Initialize Callbacks --------------------------	*/
	try {
		pCallbacks = new ImplCallbacks();
		cout << "Created callbacks object." << endl;
	}
	catch (OmneException& oEx) {
		delete pEngine;
		delete pAdmCallbacks;

		iCode = oEx.getErrorCode();
		cout << "MyCallbacks::MyCallbacks() error : " << iCode << endl;
		return 3;
	}

	pSystem->setEngine(pEngine);
	pSystem->setCallbacks(pCallbacks);

	return 0;
}
int Quant::initThreads() {
	int iCode;

	analysisManager = new AnalysisManager(pEngine, pSystem);
	orderManager = new OrderManager(pEngine, pSystem);

	iCode = orderManager->init();
	if (iCode != 0) {
		cout << endl << "Error initializing orderManager." << endl;
		return 1;
	}

	return 0;
}

/*	==========================================================================	*/

bool Quant::setAnalyManager(AnalysisManager* toAnaly) {
	analysisManager = toAnaly;
	return true;
}
bool Quant::setOrdManager(OrderManager* toOrder) {
	orderManager = toOrder;
	return true;
}