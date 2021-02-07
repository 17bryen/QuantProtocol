#include "ImplRApi.h"

using namespace std;
using namespace RApi;

/*   =====================================================================   */
/*                           custom variables                                */
/*   =====================================================================   */

static bool dumpsEnabled = true;

const int LoginStatus_NotLoggedIn = 0;
const int LoginStatus_AwaitingResults = 1;
const int LoginStatus_Failed = 2;
const int LoginStatus_Complete = 3;

/*   =====================================================================   */
/*                           custom functions                                */
/*   =====================================================================   */

bool cpytsNCharcb(tsNCharcb &dest, tsNCharcb &src) {
        dest.pData = (char*)malloc(src.iDataLen);
        dest.iDataLen = src.iDataLen;
        memcpy(dest.pData, src.pData, (size_t)dest.iDataLen);
        return true;
}

ImplAdmCallbacks::ImplAdmCallbacks(globals* callbacks) {
    callbackResponses = callbacks;
};

ImplCallbacks::ImplCallbacks(globals* callbacks, vector<Contract>* toWatch) {
    callbackResponses = callbacks;
    watchList = toWatch;
};


/*   =====================================================================   */
/*                          class definitions                                */
/*   =====================================================================   */

int ImplAdmCallbacks::Alert(AlertInfo* pInfo, void* pContext, int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */
    //if (dumpsEnabled) {
        cout << endl << endl;
        if (!pInfo->dump(&iIgnored))
        {
            cout << "error in pInfo -> dump : " << iIgnored << endl;
        }
    //}

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::AccountList(AccountListInfo* pInfo, void* pContext, int* aiCode)
{
    int iIgnored;

    if (dumpsEnabled) {
        cout << endl << endl;
        if (!pInfo->dump(&iIgnored)) {
            cout << "error in pInfo -> dump : " << iIgnored << endl;
        }
    }



    if (pInfo->iArrayLen <= 0){
        cout << "No accounts associated with this login or failed to retrieve accounts..." << endl;
        //return (NOT_OK);
    } 


    callbackResponses->pAccounts = new AccountListInfo();
    {
        callbackResponses->pAccounts->iArrayLen = pInfo->iArrayLen;
        callbackResponses->pAccounts->asAccountInfoArray = new AccountInfo[callbackResponses->pAccounts->iArrayLen];
        callbackResponses->pAccounts->iRpCode = pInfo->iRpCode;
        callbackResponses->pAccounts->sRpCode.pData = pInfo->sRpCode.pData;
        callbackResponses->pAccounts->sRpCode.iDataLen = pInfo->sRpCode.iDataLen;

        //You would loop through the array to copy account data here
        {
            AccountInfo* tempOld = pInfo->asAccountInfoArray;
            AccountInfo* tempNew = callbackResponses->pAccounts->asAccountInfoArray;

            cpytsNCharcb(tempNew[0].sAccountId, tempOld[0].sAccountId);
            cpytsNCharcb(tempNew[0].sAccountName, tempOld[0].sAccountName);
            cpytsNCharcb(tempNew[0].sFcmId, tempOld[0].sFcmId);
            cpytsNCharcb(tempNew[0].sIbId, tempOld[0].sIbId);

            callbackResponses->iSelectedAccount = 0;

            //pRmsInfo copy here
            {

            }
        }
    }

    //for(int i = 0; i < pInfo->iArrayLen; i++)
    callbackResponses->bRcvdAccountsList = true;

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::PasswordChange(PasswordChangeInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::Alert(AlertInfo* pInfo, void* pContext, int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */
    if (dumpsEnabled) {
        cout << endl << endl;
        if (!pInfo->dump(&iIgnored))
        {
            cout << "error in pInfo -> dump : " << iIgnored << endl;
        }
    }

    /*   ----------------------------------------------------------------   */
    /*   Signal when the login to the repository sub-system (agreements)    */
    /*   is complete, and what the results are.                             */

    if (pInfo->iConnectionId == REPOSITORY_CONNECTION_ID)
    {
        if (pInfo->iAlertType == ALERT_LOGIN_COMPLETE)
        {
            callbackResponses->iRepLoginStatus = LoginStatus_Complete;
        }
        else if (pInfo->iAlertType == ALERT_LOGIN_FAILED)
        {
            callbackResponses->iRepLoginStatus = LoginStatus_Failed;
        }
        else if (pInfo->iAlertType == ALERT_CONNECTION_BROKEN) {
            callbackResponses->iRepLoginStatus = LoginStatus_Failed;
        }
    }

    /*   ----------------------------------------------------------------   */
    /*   Signal when the login to the market data system (ticker plant)     */
    /*   is complete, and what the results are.                             */

    if (pInfo->iConnectionId == MARKET_DATA_CONNECTION_ID)
    {
        if (pInfo->iAlertType == ALERT_LOGIN_COMPLETE)
        {
            callbackResponses->iMdLoginStatus = LoginStatus_Complete;
        }
        else if (pInfo->iAlertType == ALERT_LOGIN_FAILED)
        {
            callbackResponses->iMdLoginStatus = LoginStatus_Failed;
        }
        else if (pInfo->iAlertType == ALERT_CONNECTION_BROKEN) {
            callbackResponses->iMdLoginStatus = LoginStatus_Failed;
        }
    }

    /*   ----------------------------------------------------------------   */
    /*   Signal when the login to the trading system                        */
    /*   is complete, and what the results are.                             */

    if (pInfo->iConnectionId == TRADING_SYSTEM_CONNECTION_ID)
    {
        if (pInfo->iAlertType == ALERT_LOGIN_COMPLETE)
        {
            callbackResponses->iTsLoginStatus = LoginStatus_Complete;
        }
        else if (pInfo->iAlertType == ALERT_LOGIN_FAILED)
        {
            callbackResponses->iTsLoginStatus = LoginStatus_Failed;
        }
        else if (pInfo->iAlertType == ALERT_CONNECTION_BROKEN) {
            callbackResponses->iTsLoginStatus = LoginStatus_Failed;
        }
    }

    /*   ----------------------------------------------------------------   */
    /*   Signal when the login to the PnL system                        */
    /*   is complete, and what the results are.                             */

    if (pInfo->iConnectionId == PNL_CONNECTION_ID)
    {
        if (pInfo->iAlertType == ALERT_LOGIN_COMPLETE)
        {
            callbackResponses->iPnlLoginStatus = LoginStatus_Complete;
        }
        else if (pInfo->iAlertType == ALERT_LOGIN_FAILED)
        {
            callbackResponses->iPnlLoginStatus = LoginStatus_Failed;
        }
        else if (pInfo->iAlertType == ALERT_CONNECTION_BROKEN) {
            callbackResponses->iPnlLoginStatus = LoginStatus_Failed;
        }
    }

    /*   ----------------------------------------------------------------   */
    /*   Signal when the login to the history system                        */
    /*   is complete, and what the results are.                             */

    if (pInfo->iConnectionId == INTRADAY_HISTORY_CONNECTION_ID)
    {
        if (pInfo->iAlertType == ALERT_LOGIN_COMPLETE)
        {
            callbackResponses->iIhLoginStatus = LoginStatus_Complete;
        }
        else if (pInfo->iAlertType == ALERT_LOGIN_FAILED)
        {
            callbackResponses->iIhLoginStatus = LoginStatus_Failed;
        }
        else if (pInfo->iAlertType == ALERT_CONNECTION_BROKEN) {
            callbackResponses->iIhLoginStatus = LoginStatus_Failed;
        }
    }
    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::AgreementList(AgreementListInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */

    cout << endl << endl;

    /*   ----------------------------------------------------------------   */

    if (!pInfo->dump(&iIgnored))
    {
        ; // do nothing ...
    }

    /*   ----------------------------------------------------------------   */

    if (!pInfo->bAccepted)
    {
        for (int i = 0; i < pInfo->iArrayLen; i++)
        {
            AgreementInfo oAg = pInfo->asAgreementInfoArray[i];

            if (oAg.bMandatory)
            {
                callbackResponses->iUnacceptedMandatoryAgreements++;
            }
        }

        callbackResponses->bRcvdUnacceptedAgreements = true;
    }

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::ExchangeList(ExchangeListInfo* pInfo, void* pContext, int* aiCode)
{
    int iIgnored;
    if (dumpsEnabled) {
        cout << endl << endl;
        if (!pInfo->dump(&iIgnored))
        {
            cout << "error in pInfo -> dump : " << iIgnored << endl;
        }
    }
    
    callbackResponses->bRcvdExchanges = true;

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::ExecutionReplay(ExecutionReplayInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}


/*   =====================================================================   */

int ImplCallbacks::LineUpdate(LineInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::OpenOrderReplay(OrderReplayInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}


/*   =====================================================================   */

int ImplCallbacks::OrderReplay(OrderReplayInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}


/*   =====================================================================   */

int ImplCallbacks::PnlReplay(PnlReplayInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::PnlUpdate(PnlInfo* pInfo, void* pContext, int* aiCode)
{
    int iIgnored;
    if (dumpsEnabled) {
        cout << endl << endl;
        if (!pInfo->dump(&iIgnored))
        {
            cout << "error in pInfo -> dump : " << iIgnored << endl;
        }
    }



    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::PriceIncrUpdate(PriceIncrInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */

    cout << endl << endl;
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::ProductRmsList(ProductRmsListInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::SingleOrderReplay(SingleOrderReplayInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::BustReport(OrderBustReport* pReport,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::CancelReport(OrderCancelReport* pReport, void* pContext, int* aiCode)
{

    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::FailureReport(OrderFailureReport* pReport,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::FillReport(OrderFillReport* pReport,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::ModifyReport(OrderModifyReport* pReport,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::NotCancelledReport(OrderNotCancelledReport* pReport,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::NotModifiedReport(OrderNotModifiedReport* pReport,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::RejectReport(OrderRejectReport* pReport,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::StatusReport(OrderStatusReport* pReport,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::TradeCorrectReport(OrderTradeCorrectReport* pReport,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::TriggerPulledReport(OrderTriggerPulledReport* pReport,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::TriggerReport(OrderTriggerReport* pReport,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::OtherReport(OrderReport* pReport,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::SodUpdate(SodReport* pReport,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::Quote(QuoteReport* pReport,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::AskQuote(AskInfo* pInfo, void* pContext, int* aiCode)
{
    int iIgnored;
    //Too fast to dump reasonably
    /*   ----------------------------------------------------------------   

    cout << endl << endl;
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }
    */

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::BestAskQuote(AskInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */

    cout << endl << endl << "Best";
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::BestBidAskQuote(BidInfo* pBid,
    AskInfo* pAsk,
    void* pContext,
    int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */

    cout << endl << endl << "Best Bid/Ask";
    if (!pBid->dump(&iIgnored))
    {
        cout << "error in pBid -> dump : " << iIgnored << endl;
    }

    if (!pAsk->dump(&iIgnored))
    {
        cout << "error in pAsk -> dump : " << iIgnored << endl;
    }

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::BestBidQuote(BidInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */

    cout << endl << endl << "Best";
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::BidQuote(BidInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    int iIgnored;

    //FAST
    /*   ----------------------------------------------------------------   

    cout << endl << endl;
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }
    */

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::BinaryContractList(BinaryContractListInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::ClosePrice(ClosePriceInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */

    cout << endl << endl;
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::ClosingIndicator(ClosingIndicatorInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */

    cout << endl << endl;
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::EndQuote(EndQuoteInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */

    cout << endl << endl;
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::EquityOptionStrategyList(EquityOptionStrategyListInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::HighPrice(HighPriceInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */

    cout << endl << endl;
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::InstrumentByUnderlying(InstrumentByUnderlyingInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::InstrumentSearch(InstrumentSearchInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::LimitOrderBook(LimitOrderBookInfo* pInfo, void* pContext, int* aiCode)
{
    int iIgnored;
    //Set rcvd to true first (Actually doesn't matter because thread will be held in this function) 
    //Dumping this takes too long
    /*   ----------------------------------------------------------------   

    cout << endl << endl;
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }
    */

    OrderBook* temp = watchList->at(0).book;

    if (!callbackResponses->bRcvdLimitOrderBook) {

        temp->askArrayLength = pInfo->iAskArrayLen;
        temp->bidArrayLength = pInfo->iBidArrayLen;


        temp->askPriceArray = (double*)malloc(8 * temp->askArrayLength);
        temp->askOrdersArray = (int*)malloc(4 * temp->askArrayLength);
        temp->askSizeArray = (int*)malloc(4 * temp->askArrayLength);

        memcpy(temp->askPriceArray, pInfo->adAskPriceArray, 8 * temp->askArrayLength);
        memcpy(temp->askOrdersArray, pInfo->aiAskNumOrdersArray, 4 * temp->askArrayLength);
        memcpy(temp->askSizeArray, pInfo->aiAskSizeArray, 4 * temp->askArrayLength);


        temp->bidPriceArray = (double*)malloc(8 * pInfo->iBidArrayLen);
        temp->bidOrdersArray = (int*)malloc(4 * pInfo->iBidArrayLen);
        temp->bidSizeArray = (int*)malloc(4 * pInfo->iBidArrayLen);

        memcpy(temp->bidPriceArray, pInfo->adBidPriceArray, 8 * temp->askArrayLength);
        memcpy(temp->bidOrdersArray, pInfo->aiBidNumOrdersArray, 4 * temp->askArrayLength);
        memcpy(temp->bidSizeArray, pInfo->aiBidSizeArray, 4 * temp->askArrayLength);

        callbackResponses->bRcvdLimitOrderBook = true;

    }
    else {
        cout << endl << endl << "SUBSEQUENT ORDER BOOK RECEIVED: Comparative lengths \nCurrent Ask: " << temp->askArrayLength
            << " Bid: " << temp->bidArrayLength << "\nNew Ask: " << pInfo->iAskArrayLen << " Bid: " << pInfo->iBidArrayLen << endl;
    }

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::LowPrice(LowPriceInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */

    cout << endl << endl;
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::MarketMode(MarketModeInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */

    cout << endl << endl;
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::OpenInterest(OpenInterestInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */

    cout << endl << endl;
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::OpenPrice(OpenPriceInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */

    cout << endl << endl;
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::OpeningIndicator(OpeningIndicatorInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */

    cout << endl << endl;
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::OptionList(OptionListInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::RefData(RefDataInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::SettlementPrice(SettlementPriceInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */

    cout << endl << endl;
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::Strategy(StrategyInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::StrategyList(StrategyListInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::TradeCondition(TradeInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */

    cout << endl << endl;
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::TradePrint(TradeInfo* pInfo, void* pContext, int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */
    cout << "Trade info received..." << endl;
    cout << endl << endl;

    //ZOOM
    /*
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }
    */

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::TradeReplay(TradeReplayInfo* pInfo, void* pContext, int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */
    cout << "Trade info received..." << endl;
    if (dumpsEnabled) {
        cout << endl << endl;
        if (!pInfo->dump(&iIgnored))
        {
            cout << "error in pInfo -> dump : " << iIgnored << endl;
        }
    }

    callbackResponses->bRcvdReplayTrades = true;

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::TradeRoute(TradeRouteInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::TradeRouteList(TradeRouteListInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::TradeVolume(TradeVolumeInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   */

    cout << endl << endl;
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }

    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::Bar(BarInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::BarReplay(BarReplayInfo* pInfo,
    void* pContext,
    int* aiCode)
{
    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */
