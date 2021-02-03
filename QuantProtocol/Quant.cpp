#include "Quant.h"
#include "Main.h"

using namespace std;
using namespace RApi;

/*==============================================================================*/
/*							Define Member Functions							 	*/
/*==============================================================================*/


/* Constructor for main obj class. Initializes default connect points, however 
*	object pointers are left as NULLS so that their initialization may return proper 
*	error values. 
*/
Quant::Quant(char* md, char* ts, char* pnl, char* ih) {

/*	--------------- Provide Deault Values to Connect Points --------------	*/

	if (md != NULL) {
		mdCnnctPoint.pData = (char*)md;
		mdCnnctPoint.iDataLen = (int)strlen(mdCnnctPoint.pData);
	} else {
		mdCnnctPoint.pData = (char*)"login_agent_tp_r01c"; //Use login_agent_tp_agg_r01c for aggregated data
		mdCnnctPoint.iDataLen = (int)strlen(mdCnnctPoint.pData);
	}
	
	if (ts != NULL) {
		tsCnnctPoint.pData = (char*)ts;
		tsCnnctPoint.iDataLen = (int)strlen(tsCnnctPoint.pData);
	} else {
		tsCnnctPoint.pData = (char*)"login_agent_prodc";
		tsCnnctPoint.iDataLen = (int)strlen(tsCnnctPoint.pData);
	}
	
	if (pnl != NULL) {
		pnlCnnctPoint.pData = (char*)pnl;
		pnlCnnctPoint.iDataLen = (int)strlen(pnlCnnctPoint.pData);
	} else {
		pnlCnnctPoint.pData = (char*)"login_agent_pnl_sslc";
		pnlCnnctPoint.iDataLen = (int)strlen(pnlCnnctPoint.pData);
	}
	
	if (ih != NULL) {
		ihCnnctPoint.pData = (char*)ih;
		ihCnnctPoint.iDataLen = (int)strlen(ihCnnctPoint.pData);
	} else {
		ihCnnctPoint.pData = (char*)"login_agent_historyc";
		ihCnnctPoint.iDataLen = (int)strlen(ihCnnctPoint.pData);
	}

	rpCnnctPoint.pData = (char*)"login_agent_repositoryc";
	rpCnnctPoint.iDataLen = (int)strlen(rpCnnctPoint.pData);

/*	-------------- Create Struct to Hold Callback Responses --------------	*/

	callbackResponses = new globals();

/*	---------------- User, Pass, and Ptrs are set to NULL ----------------	*/

	user.iDataLen = 0;
	pass.iDataLen = 0;

	pEngine = nullptr;
	pAdmCallbacks = nullptr;
	pCallbacks = nullptr;
}

/* Deconstructor to properly allocate memory back from complex objects.
*/
Quant::~Quant() {
	/*	--------------------- Delete Member Variables ------------------------	*/
	

	/*	---------------------- Delete Member Objects -------------------------	*/
	delete pCallbacks;
	delete pEngine;
	delete pAdmCallbacks;

	delete callbackResponses;
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
		pAdmCallbacks = new ImplAdmCallbacks(callbackResponses);
		cout << "Created AdmCallbacks object... ";
	}
	catch (OmneException& oEx) {
		iCode = oEx.getErrorCode();
		cout << endl << "ImplAdmCallbacks::ImplAdmCallbacks() error : " << iCode << endl;
		return 1;
	}


/*	---------------------------- Initialize REngine --------------------------	*/
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
		pCallbacks = new ImplCallbacks(callbackResponses);
		cout << "Created callbacks object." << endl;
	}
	catch (OmneException& oEx) {
		delete pEngine;
		delete pAdmCallbacks;

		iCode = oEx.getErrorCode();
		cout << "MyCallbacks::MyCallbacks() error : " << iCode << endl;
		return 3;
	}

	return 0;
}

/* Logs the Quant object into the connect points using stored values. Be sure
*	to call setUser() and setPass() prior to this point.
*/
int Quant::login() {
	int iCode;
	LoginParams oLoginParams;

	oLoginParams.pCallbacks = pCallbacks;

	oLoginParams.sUser = user;
	oLoginParams.sPassword = pass;
	oLoginParams.sMdUser = user;
	oLoginParams.sMdPassword = pass;
	oLoginParams.sIhUser = user;
	oLoginParams.sIhPassword = pass;

	oLoginParams.sMdCnnctPt = mdCnnctPoint;
	oLoginParams.sTsCnnctPt = tsCnnctPoint;
	oLoginParams.sPnlCnnctPt = pnlCnnctPoint;
	oLoginParams.sIhCnnctPt = ihCnnctPoint;

	if (!pEngine->login(&oLoginParams, &iCode)) {
		cout << endl << "REngine::login() error : " << iCode << endl;
		return 1;
	}

	while ((callbackResponses->iMdLoginStatus != LoginStatus_Complete && callbackResponses->iMdLoginStatus != LoginStatus_Failed) ||
		(callbackResponses->iTsLoginStatus != LoginStatus_Complete && callbackResponses->iTsLoginStatus != LoginStatus_Failed) ||
		(callbackResponses->iPnlLoginStatus != LoginStatus_Complete && callbackResponses->iPnlLoginStatus != LoginStatus_Failed) ||
		(callbackResponses->iIhLoginStatus != LoginStatus_Complete && callbackResponses->iIhLoginStatus != LoginStatus_Failed)) 
	{


	}
}