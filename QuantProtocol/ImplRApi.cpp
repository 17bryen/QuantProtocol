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

ImplCallbacks::ImplCallbacks(globals* callbacks, vector<Account>* toAccount, vector<Contract>* toWatch) {
    callbackResponses = callbacks;
    accounts = toAccount;
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
        return (NOT_OK);
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

            for (int i = 0; i < pInfo->iArrayLen; i++) {
                cpytsNCharcb(tempNew[i].sAccountId, tempOld[i].sAccountId);
                cpytsNCharcb(tempNew[i].sAccountName, tempOld[i].sAccountName);
                cpytsNCharcb(tempNew[i].sFcmId, tempOld[i].sFcmId);
                cpytsNCharcb(tempNew[i].sIbId, tempOld[i].sIbId);
            }

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
    
    //callbackResponses->bRcvdExchanges = true;

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
    /*   ----------------------------------------------------------------   */

    if (pInfo->eAccountBalance == VALUE_STATE_USE)
        for (int i = 0; i < accounts->size(); i++)
            if (accounts->at(i).accId.pData == pInfo->oAccount.sAccountId.pData)
                accounts->at(i).accBalance = pInfo->dAccountBalance;

    /*   ----------------------------------------------------------------   */
    //ADD CODE TO ACCURATELY TRACK CURRENT POSITIONS

    /*   ----------------------------------------------------------------   */

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
    if (pInfo->iUpdateType == UPDATE_TYPE_SOLO || pInfo->iUpdateType == UPDATE_TYPE_BEGIN)
        watchList->at(0).Dom.lock();

    watchList->at(0).book->updateAsk(pInfo);
    
    if (pInfo->iUpdateType == UPDATE_TYPE_SOLO || pInfo->iUpdateType == UPDATE_TYPE_END)
        watchList->at(0).Dom.unlock();
    /*   ----------------------------------------------------------------   */

    *aiCode = API_OK;
    return (OK);
}

/*   =====================================================================   */

int ImplCallbacks::BestAskQuote(AskInfo* pInfo, void* pContext, int* aiCode)
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

    if (pInfo->iUpdateType == UPDATE_TYPE_SOLO || pInfo->iUpdateType == UPDATE_TYPE_BEGIN)
        watchList->at(0).Dom.lock();

    watchList->at(0).book->updateBid(pInfo);
    
    if (pInfo->iUpdateType == UPDATE_TYPE_SOLO || pInfo->iUpdateType == UPDATE_TYPE_END)
        watchList->at(0).Dom.unlock();

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

int ImplCallbacks::EndQuote(EndQuoteInfo* pInfo, void* pContext, int* aiCode)
{
    int iIgnored;

    /*   ----------------------------------------------------------------   

    cout << endl << endl;
    if (!pInfo->dump(&iIgnored))
    {
        cout << "error in pInfo -> dump : " << iIgnored << endl;
    }
    */

    watchList->at(0).Dom.unlock();

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

    if (pInfo->iType == UPDATE_TYPE_SOLO || pInfo->iType == UPDATE_TYPE_BEGIN)
        watchList->at(0).Dom.lock();

    /*   ----------------------------------------------------------------   */

    if (!callbackResponses->bRcvdLimitOrderBook) {

        watchList->at(0).book->updateBook(pInfo);
        callbackResponses->bRcvdLimitOrderBook = true;

    }
    else {
        watchList->at(0).book->updateBook(pInfo);

        cout << endl << endl << "SUBSEQUENT ORDER BOOK RECEIVED:" << endl;
    }

    /*   ----------------------------------------------------------------   */

    if (pInfo->iType == UPDATE_TYPE_SOLO || pInfo->iType == UPDATE_TYPE_END)
        watchList->at(0).Dom.unlock();

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
    int iIgnored = 0;

    /*   ----------------------------------------------------------------   */

    //ZOOM
    /*
    if (pInfo->iType == MD_HISTORY_CB) {
        if (!pInfo->dump(&iIgnored))
        {
            cout << "error in pInfo -> dump : " << iIgnored << endl;
        }
    }
    */
    if (pInfo->iSourceSsboe % 60 == 0)
        cout << pInfo->iSourceSsboe << endl;
    cout << "replay trade received" << endl;

    watchList->at(0).Tape.lock();

    watchList->at(0).flow->updateTrades(pInfo);

    watchList->at(0).Tape.unlock();

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

int ImplCallbacks::TradeRoute(TradeRouteInfo* pInfo, void* pContext, int* aiCode)
{
    int iIgnored;
    pInfo->dump(&iIgnored);
    
    *aiCode = API_OK;
    return(OK);
}

/*   =====================================================================   */

int ImplCallbacks::TradeRouteList(TradeRouteListInfo* pInfo, void* pContext, int* aiCode)
{
    int iIgnored;
    pInfo->dump(&iIgnored);

    accounts->at(0).tradeRoutes->iArrayLen = pInfo->iArrayLen;
    accounts->at(0).tradeRoutes->asTradeRouteInfoArray = new TradeRouteInfo[pInfo->iArrayLen];
    TradeRouteInfo* temp = accounts->at(0).tradeRoutes->asTradeRouteInfoArray;

    for (int i = 0; i < pInfo->iArrayLen; i++) {
        temp[i].iType = pInfo->asTradeRouteInfoArray[i].iType;
        cpytsNCharcb(temp[i].sDefault, pInfo->asTradeRouteInfoArray[i].sDefault);
        cpytsNCharcb(temp[i].sExchange, pInfo->asTradeRouteInfoArray[i].sExchange);
        cpytsNCharcb(temp[i].sFcmId, pInfo->asTradeRouteInfoArray[i].sFcmId);
        cpytsNCharcb(temp[i].sIbId, pInfo->asTradeRouteInfoArray[i].sIbId);
        cpytsNCharcb(temp[i].sStatus, pInfo->asTradeRouteInfoArray[i].sStatus);
        cpytsNCharcb(temp[i].sTradeRoute, pInfo->asTradeRouteInfoArray[i].sTradeRoute);
    }

    callbackResponses->bRcvdTradeRoutes = true;

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
