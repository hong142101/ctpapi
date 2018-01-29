#include "API.h"
#include <fstream>

/////////////////////////////////////////////////////////////////////////
CTPAPI::CTPAPI(EventEngine *eventengine)
{
	this->mdapi = new MdApi(this);
	this->tdapi = new TdApi(this);
	this->eventengine = eventengine;
	memset(pQryInstrument, 0, sizeof(*pQryInstrument));
	pReqUserLoginField = CommonFuction::get_user_info("API.xml");
}

CTPAPI::~CTPAPI()
{
	delete this->mdapi;
	this->mdapi = NULL;
	delete this->tdapi;
	this->mdapi = NULL;
}

int CTPAPI::ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID)
{
	Sleep(1000);
	int i = this->tdapi->ReqQryInstrument(pQryInstrument, nRequestID);
	return i;
}

const char* CTPAPI::GetTradingDay()
{
	return this->mdapi->GetTradingDay();
}

void CTPAPI::ctp_connect()
{
	this->mdapi->init();
	this->mdapi->connect();
	this->tdapi->init();
	this->tdapi->connect();
	int i = 1;
	while (i != 0)
	{
		i = this->ReqQryInstrument(this->pQryInstrument, ++nReq);
	}
	std::cout << "ReqQryInstrument succeeded!\n";
	while (!this->tdapi->get_if_instrument_finished())
	{ }
	int s = this->SubscribeAllMarketData();
}

void CTPAPI::join()
{
	this->mdapi->join();
	this->tdapi->join();
}

int CTPAPI::SubscribeAllMarketData()
{
	for (auto id = this->tdapi->instrument_vector.begin(); id != this->tdapi->instrument_vector.end(); ++id)
	{
		char *contract[1] = { (char*)id->c_str() };
		std::printf("subscribe contract: %s\n", contract[0]);
		int i = this->mdapi->SubscribeMarketData(contract, 1);
	}
	return 0;
}

int CTPAPI::SubscribeMarketData(std::vector<std::string> &InstrumentID)
{
	for (auto id = InstrumentID.begin(); id != InstrumentID.end(); ++id)
	{
		char *contract[1] = { (char*)id->c_str() };
		std::printf("contract: %s\n", contract[0]);
		int i = this->mdapi->SubscribeMarketData(contract, 1);
	}
	return 0;
}
