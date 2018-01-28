#include <iostream>
#include <string>
#include <map>
#include <Windows.h>
#include <exception>
#include "API.h"


/////////////////////////////////////////////////////////////////////////
void MdApi::OnFrontConnected()
{
	std::cout << "OnFrontConnected succeed" << std::endl;
	frontconnected = true;
}

void MdApi::OnFrontDisconnected(int nReason)
{
	std::cout << "OnFrontDisconnected succeed\n reason is " << std::endl;
}

///心跳超时警告。当长时间未收到报文时，该方法被调用。
///@param nTimeLapse 距离上次接收报文的时间
void MdApi::OnHeartBeatWarning(int nTimeLapse)
{
	std::cout << "OnHeartBeatWarning succeed\n TimeLapse is " << std::endl;
}


///登录请求响应
void MdApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "OnRspUserLogin succeed" << std::endl;
	userLogined = true;
}

///登出请求响应
void MdApi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "OnRspUserLogout succeed" << std::endl;
}

///错误应答
void MdApi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::shared_ptr<Event_Error>e = std::make_shared<Event_Error>();
	e->error = *pRspInfo;
	this->ctpapi->onError(e);

}

///订阅行情应答
void MdApi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//std::cout << "OnRspSubMarketData succeeded: " << pSpecificInstrument->InstrumentID << std::endl;
	if (pRspInfo->ErrorID != 0)
		std::cout << "ERROR pRspInfo : " << pRspInfo->ErrorMsg << std::endl;
}

///取消订阅行情应答
void MdApi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "OnRspUnSubMarketData succeed" << std::endl;
}

///订阅询价应答
void MdApi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "OnRspSubForQuoteRsp succeed" << std::endl;
}

///取消订阅询价应答
void MdApi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "OnRspUnSubForQuoteRsp succeed" << std::endl;
}

///深度行情通知
void MdApi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	std::shared_ptr<Event_Tick>e = std::make_shared<Event_Tick>(pDepthMarketData->InstrumentID);
	e->tick = *pDepthMarketData;
	//if (pDepthMarketData->AskPrice1 < 1.0e308) std::printf("AskPrice1: %f\n", pDepthMarketData->AskPrice1); else std::printf("AskPrice1: %i\n", 0);
	this->ctpapi->onTick(e);
}

int MdApi::SubscribeMarketData(char *ppInstrumentID[], int nCount)
{
	int i = this->api->SubscribeMarketData(ppInstrumentID, nCount);
	return i;
}

int MdApi::UnSubscribeMarketData(char *ppInstrumentID[], int nCount)
{
	int i = this->api->UnSubscribeMarketData(ppInstrumentID, nCount);
	return i;
}

int MdApi::SubscribeForQuoteRsp(char *ppInstrumentID[], int nCount)
{
	int i = this->api->SubscribeForQuoteRsp(ppInstrumentID, nCount);
	return i;
}

int MdApi::UnSubscribeForQuoteRsp(char *ppInstrumentID[], int nCount)
{
	int i = this->api->UnSubscribeForQuoteRsp(ppInstrumentID, nCount);
	return i;
}

int MdApi::ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID)
{
	int i = this->api->ReqUserLogout(pUserLogout, nRequestID);
	return i;
}


/////////////////////////////////////////////////////////////////////////
void TdApi::OnFrontConnected()
{
	std::cout << "OnFrontConnected succeed" << std::endl;
	frontconnected = true;
}

void TdApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "OnRspUserLogin succeed!" << std::endl;
	std::printf("UserID: %s\n", pRspUserLogin->UserID);
	std::printf("TradingDay: %s\n", pRspUserLogin->TradingDay);
	std::printf("LoginTime: %s\n", pRspUserLogin->LoginTime);
	std::printf("FrontID: %i\n", pRspUserLogin->FrontID);
	std::printf("SessionID: %i\n", pRspUserLogin->SessionID);
	std::printf("MaxOrderRef: %s\n", pRspUserLogin->MaxOrderRef);
	userLogined = true;
}

void TdApi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "OnRspSettlementInfoConfirm succeed!" << std::endl;
	std::printf("BrokerID: %s\n", pSettlementInfoConfirm->BrokerID);
	std::printf("InvestorID: %s\n", pSettlementInfoConfirm->InvestorID);
	std::printf("ConfirmDate: %s\n", pSettlementInfoConfirm->ConfirmDate);
	std::printf("ConfirmTime: %s\n", pSettlementInfoConfirm->ConfirmTime);
	this->settlementinfo_confirmed = true;
}

void TdApi::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "OnRspQrySettlementInfo succeed!" << std::endl;
	this->settlementinfo_finished = true;
	//std::printf("TradingDay: %s\n", pSettlementInfo->TradingDay);
	//std::printf("SettlementID: %i\n", pSettlementInfo->SettlementID);
	//std::printf("BrokerID: %s\n", pSettlementInfo->BrokerID);
	//std::printf("InvestorID: %s\n", pSettlementInfo->InvestorID);
	//std::printf("SequenceNo: %i\n", pSettlementInfo->SequenceNo);
	//std::printf("Content: %s\n", pSettlementInfo->Content);
}

void TdApi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pInstrument->ProductClass == THOST_FTDC_PC_Futures)
	{
		std::cout << "onrspqry InstrumentID: " << pInstrument->InstrumentID << std::endl;
		this->spi_mutex.lock();
		this->instrument_vector.push_back(pInstrument->InstrumentID);
		this->instrument_info[pInstrument->InstrumentID] = pInstrument;
		this->instrument_exchange[pInstrument->InstrumentID] = pInstrument->ExchangeID;
		this->instrument_product[pInstrument->InstrumentID] = pInstrument->ProductID;
		this->spi_mutex.unlock();
	}
	if (bIsLast)
	{
		this->onrspqryinstrument_finished = true;
		std::cout << "OnRspQryInstrument succeed!" << std::endl;
	}
}

void TdApi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	std::shared_ptr<Event_Order>e = std::make_shared<Event_Order>();
	e->order = *pOrder;
	this->ctpapi->onOrder(e);
}

void TdApi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	std::shared_ptr<Event_Trade>e = std::make_shared<Event_Trade>();
	e->trade = *pTrade;
	this->ctpapi->onTrade(e);
}

void TdApi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::shared_ptr<Event_Position>e = std::make_shared<Event_Position>();
	e->position = *pInvestorPosition;
	this->ctpapi->onTrade(e);
}

void TdApi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::shared_ptr<Event_Account>e = std::make_shared<Event_Account>();
	e->account = *pTradingAccount;
	this->ctpapi->onTrade(e);
}

void TdApi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::shared_ptr<Event_Error>e = std::make_shared<Event_Error>();
	e->error = *pRspInfo;
	this->ctpapi->onError(e);

}


int TdApi::ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID)
{
	int i = this->api->ReqUserLogout(pUserLogout, nRequestID);
	return i;
}

int TdApi::ReqOrderInsert(CThostFtdcInputOrderField *pInputOrder, int nRequestID)
{
	int i = this->api->ReqOrderInsert(pInputOrder, nRequestID);
	return i;
}

int TdApi::ReqParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, int nRequestID)
{
	int i = this->api->ReqParkedOrderInsert(pParkedOrder, nRequestID);
	return i;
}

int TdApi::ReqParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, int nRequestID)
{
	int i = this->api->ReqParkedOrderAction(pParkedOrderAction, nRequestID);
	return i;
}

int TdApi::ReqOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, int nRequestID)
{
	int i = this->api->ReqOrderAction(pInputOrderAction, nRequestID);
	return i;
}

int TdApi::ReqRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, int nRequestID)
{
	int i = this->api->ReqRemoveParkedOrder(pRemoveParkedOrder, nRequestID);
	return i;
}

int TdApi::ReqRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, int nRequestID)
{
	int i = this->api->ReqRemoveParkedOrderAction(pRemoveParkedOrderAction, nRequestID);
	return i;
}

int TdApi::ReqQryInvestorPosition(CThostFtdcQryInvestorPositionField *pQryInvestorPosition, int nRequestID)
{
	int i = this->api->ReqQryInvestorPosition(pQryInvestorPosition, nRequestID);
	return i;
}

int TdApi::ReqQryTradingAccount(CThostFtdcQryTradingAccountField *pQryTradingAccount, int nRequestID)
{
	int i = this->api->ReqQryTradingAccount(pQryTradingAccount, nRequestID);
	return i;
}

int TdApi::ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID)
{
	int i = this->api->ReqQryInstrumentMarginRate(pQryInstrumentMarginRate, nRequestID);
	return i;
}

int TdApi::ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID)
{
	int i = this->api->ReqQryInstrumentCommissionRate(pQryInstrumentCommissionRate, nRequestID);
	return i;
}

int TdApi::ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID)
{
	int i = this->api->ReqQryInstrument(pQryInstrument, nRequestID);
	return i;
}


/////////////////////////////////////////////////////////////////////////
void MdApi::init()
{
	if (!CommonFuction::checkExist("./CtpFlowFiles"))
	{
		while (!CommonFuction::createDirectory("./CtpFlowFiles"))
		{ }
	}
	CThostFtdcMdApi* api = CThostFtdcMdApi::CreateFtdcMdApi("./CtpFlowFiles/ctpmd");
	this->api = api;
	this->api->RegisterSpi(this);
}

void MdApi::connect()
{
	std::vector<char*> *mdadress = CommonFuction::get_servers_adress("API.xml", "MarketData");
	for (auto ad = mdadress->begin(); ad != mdadress->end(); ++ad)
	{
		std::cout << *ad << std::endl;
		this->api->RegisterFront(*ad);
	}
	this->api->Init();
	std::printf("init succeeded!\n");
	CThostFtdcReqUserLoginField *pReqUserLoginField = CommonFuction::get_user_info("API.xml");

	while (this->frontconnected != true)
	{ }
	int i = api->ReqUserLogin(pReqUserLoginField, ++this->ctpapi->nReq);
	while( this->userLogined != true)
	{ }
	//char *ppInstrumentID[] = {(char*)"rb1805"};
	//std::printf("%zi", sizeof(ppInstrumentID));
	//this->api->SubscribeMarketData(ppInstrumentID, 1);

}

int MdApi::join()
{
	int i = this->api->Join();
	return i;
}


/////////////////////////////////////////////////////////////////////////
void TdApi::init()
{
	if (!CommonFuction::checkExist("./CtpFlowFiles"))
	{
		while (!CommonFuction::createDirectory("./CtpFlowFiles"))
		{ }
	}
	CThostFtdcTraderApi* api = CThostFtdcTraderApi::CreateFtdcTraderApi("./CtpFlowFiles/ctptd");
	this->api = api;
	this->api->RegisterSpi(this);
}

void TdApi::connect()
{
	std::vector<char*> *tdadress = CommonFuction::get_servers_adress("API.xml", "Trader");
	for (auto ad = tdadress->begin(); ad != tdadress->end(); ++ad)
	{
		std::cout << *ad << std::endl;
		this->api->RegisterFront(*ad);
	}
	this->api->SubscribePrivateTopic(THOST_TERT_RESTART);
	this->api->SubscribePrivateTopic(THOST_TERT_RESUME);
	this->api->Init();
	std::printf("td init succeeded!\n");
	CThostFtdcReqUserLoginField *pReqUserLoginField = CommonFuction::get_user_info("API.xml");

	CThostFtdcQrySettlementInfoField *pQrySettlementInfo = CommonFuction::get_settlement_info<CThostFtdcQrySettlementInfoField>("API.xml");

	CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm = CommonFuction::get_settlement_info<CThostFtdcSettlementInfoConfirmField>("API.xml");

	//CThostFtdcQryInstrumentField pQryInstrument;
	//memset(&pQryInstrument, 0, sizeof(pQryInstrument));

	while (this->frontconnected != true)
	{ }
	int i = api->ReqUserLogin(pReqUserLoginField, ++this->ctpapi->nReq);
	while (this->userLogined != true)
	{ }
	int r = this->api->ReqQrySettlementInfo(pQrySettlementInfo, ++this->ctpapi->nReq);
	while (this->settlementinfo_finished != true)
	{ }
	int q = this->api->ReqSettlementInfoConfirm(pSettlementInfoConfirm, ++this->ctpapi->nReq);
	while (this->settlementinfo_confirmed != true)
	{ }
	//Sleep(1000);
	//int in = this->api->ReqQryInstrument(pQryInstrument, 5);

}

int TdApi::join()
{
	int i = this->api->Join();
	return i;
}

