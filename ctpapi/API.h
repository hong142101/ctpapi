#pragma once
#ifndef API_H
#define API_H

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64) || defined(_WINDOWS)
#define __PLATFORM_WINDOWS__
#else
#define __PLATFORM_LINUX__
#endif

#ifdef __PLATFORM_WINDOWS__
#ifdef  CTPAPI_EXPORTS
#define CTPAPI_API __declspec(dllexport)
#else
#define CTPAPI_API __declspec(dllimport)
#endif
#endif

#ifdef __PLATFORM_LINUX__
#define CTPAPI_API
#endif

#include <Windows.h>
#include "CTPAPI.h"


class CTPAPI_API CTPAPI
{
public:
	std::atomic_int nReq = 1;
	std::vector<std::string> InstrumentID;

	//查询全部合约
	int ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID);

	//订阅全部合约行情
	int SubscribeAllMarketData();

	//订阅全部合约行情
	int SubscribeMarketData(std::vector<std::string> &InstrumentID);

	const char* GetTradingDay();

	void onTick(std::shared_ptr<Event>e)
	{
		this->eventengine->put(e);
	}

	void onOrder(std::shared_ptr<Event>e)
	{
		this->eventengine->put(e);
	}

	void onTrade(std::shared_ptr<Event>e)
	{
		this->eventengine->put(e);
	}

	void onPosition(std::shared_ptr<Event>e)
	{
		this->eventengine->put(e);
	}

	void onAccount(std::shared_ptr<Event_Account>e)
	{
		this->eventengine->put(e);
	}

	void onError(std::shared_ptr<Event_Error>e)
	{
		this->eventengine->put(e);
	}

	explicit CTPAPI(EventEngine *eventengine);

	~CTPAPI();

	void ctp_connect();

	void join();

	const std::vector<std::string> *get_all_InstrumentID()
	{
		return &this->tdapi->instrument_vector;
	}

	const std::map<std::string, CThostFtdcInstrumentField> *get_instrument_info()
	{
		return &this->tdapi->instrument_info;
	}

private:
	MdApi * mdapi = nullptr;
	TdApi *tdapi = nullptr;
	EventEngine *eventengine = nullptr;
	CThostFtdcQryInstrumentField *pQryInstrument = new CThostFtdcQryInstrumentField;
	CThostFtdcReqUserLoginField *pReqUserLoginField = new CThostFtdcReqUserLoginField;
};

#endif // !API_H
