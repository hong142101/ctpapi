#pragma once
#ifndef COMFUC_H
#define COMFUC_H

#ifndef F_OK
#define F_OK 0
#endif
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <boost\property_tree\xml_parser.hpp>
#include <boost\filesystem\path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"

#ifdef WIN32
#include <direct.h>
#include <io.h>
#endif

#ifdef linux
#include <dirent.h>
#include <sys/stat.h>
#endif

class CommonFuction
{
public:
	//win和linux通用操作类
	static bool checkExist(const std::string &path)
	{
#ifdef linux
		int result = access(path.c_str(), F_OK);

		if (result == -1) return false;
		else if (result == 0) return true;
		else return false;

#endif
#ifdef WIN32
		int result = _access(path.c_str(), F_OK);
		if (result == -1) return false;
		else if (result == 0) return true;
		else return false;
#endif
	}

	static bool createDirectory(const std::string &path)
	{
#ifdef WIN32
		int result = _mkdir(path.c_str());
		if (result == -1)
		{
			return false;
		}
		else if (result == 0)
		{
			return true;
		}
		return false;
#endif
#ifdef linux
		int result = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if (result == -1)
		{
			return false;
		}
		else if (result == 0)
		{
			return true;
		}
#endif
	}

	static CThostFtdcReqUserLoginField *get_user_info(const std::string filename)
	{
		boost::property_tree::ptree* tree = new boost::property_tree::ptree;
		boost::property_tree::ptree* root = new boost::property_tree::ptree;
		if (!checkExist(filename))
		{
			std::cout << "API.xml is not exist!" << std::endl;
			std::getchar();
		}
		boost::property_tree::read_xml(filename, *tree);
#ifdef CTPAPI_H
		*root = tree->get_child("ctp");
#endif
		std::map<std::string, std::string> config;
		for (boost::property_tree::ptree::iterator it = root->begin(); it != root->end(); it++)
		{
			boost::property_tree::ptree node = it->second;
			std::string name = it->first;
			std::string value = node.data();
			config[name] = value;
		}
		delete tree;
		delete root;
		CThostFtdcReqUserLoginField *userinfo = new CThostFtdcReqUserLoginField;
		memset(userinfo, 0, sizeof(*userinfo));
		strcpy_s(userinfo->BrokerID, sizeof(config["BrokerID"]) + 1, config["BrokerID"].c_str());
		strcpy_s(userinfo->UserID, sizeof(config["UserID"]) + 1, config["UserID"].c_str());
		strcpy_s(userinfo->Password, sizeof(config["Password"]) + 1, config["Password"].c_str());
		return userinfo;
	}

	static std::vector<char*> *get_servers_adress(const std::string filename, const std::string adress)
	{
		boost::property_tree::ptree* tree = new boost::property_tree::ptree;
		boost::property_tree::ptree* root = new boost::property_tree::ptree;
		if (!checkExist(filename))
		{
			std::cout << filename << " is not exist!\n";
			throw filename;
		}
		boost::property_tree::read_xml(filename, *tree);
#ifdef CTPAPI_H
		*root = tree->get_child("ctp");
#endif
		*root = root->get_child("servers");
		if (adress == "Trader" or adress == "MarketData") *root = root->get_child(adress);
		else
		{
			std::cout << "servers name wrong!\n";
			throw adress;
		}
		std::vector<char*> *tdadress = new std::vector<char*>;
		for (boost::property_tree::ptree::iterator it = root->begin(); it != root->end(); it++)
		{
			boost::property_tree::ptree node = it->second;
			std::string value = node.data();
			tdadress->push_back((char*)value.c_str());
		}
		return tdadress;
	}

	template<typename STRUCT>
	static STRUCT *get_settlement_info(const std::string filename)
	{
		boost::property_tree::ptree* tree = new boost::property_tree::ptree;
		boost::property_tree::ptree* root = new boost::property_tree::ptree;
		if (!checkExist(filename))
		{
			std::cout << "API.xml is not exist!" << std::endl;
			std::getchar();
		}
		boost::property_tree::read_xml(filename, *tree);
#ifdef CTPAPI_H
		*root = tree->get_child("ctp");
#endif
		std::map<std::string, std::string> config;
		for (boost::property_tree::ptree::iterator it = root->begin(); it != root->end(); it++)
		{
			boost::property_tree::ptree node = it->second;
			std::string name = it->first;
			std::string value = node.data();
			config[name] = value;
		}
		delete tree;
		delete root;
		STRUCT *userinfo = new STRUCT;
		memset(userinfo, 0, sizeof(*userinfo));
		strcpy_s(userinfo->BrokerID, sizeof(config["BrokerID"]) + 1, config["BrokerID"].c_str());
		strcpy_s(userinfo->InvestorID, sizeof(config["UserID"]) + 1, config["UserID"].c_str());
		return userinfo;
	}

	static bool folder_not_exists_and_create(std::string path)
	{
		while (!boost::filesystem::exists(path))
		{
			boost::filesystem::create_directories(path);
		}
		return true;
	}

	static bool file_if_exists(std::string path)
	{
		return boost::filesystem::exists(path);
	}

	static std::string get_now_datetime()
	{
		struct tm t;   //tm结构指针
		time_t now;  //声明time_t类型变量
		time(&now);      //获取系统日期和时间
		localtime_s(&t, &now);   //获取当地日期和时间
		std::string year = std::to_string(t.tm_year + 1900);
		std::string month = std::to_string(t.tm_mon + 1);
		std::string day = std::to_string(t.tm_mday);
		std::string hour = std::to_string(t.tm_hour);
		std::string minute = std::to_string(t.tm_min);
		std::string second = std::to_string(t.tm_sec);
		std::string datetime = year + "-" + month + "-" + day + " " + hour + ":" + minute + ":" + second;
		return datetime;
	}
};

#define EVENT_QUIT "equit"//退出事件
#define EVENT_TIMER "etimer"//循环1秒钟事件，用来不断的查持仓，防止CTP流控

class Event //结构中定义事件
{
public:
	Event(std::string type) : m_eventtype(type)
	{}
	std::string GetEventType()
	{
		return m_eventtype;
	}
private:
	std::string m_eventtype;
};

class Event_Exit :public Event
{
public:
	Event_Exit() :Event(EVENT_QUIT)
	{}
};

class Event_Timer :public Event
{
public:
	Event_Timer() :Event(EVENT_TIMER)
	{}
};

class Event_Tick : public Event
{
public:
	Event_Tick(std::string sec_type) :Event("etick" + sec_type)
	{}
	CThostFtdcDepthMarketDataField tick;
};

class Event_Order : public Event
{
public:
	Event_Order() :Event("eorder")
	{}
	CThostFtdcOrderField order;
};

class Event_Trade : public Event
{
public:
	Event_Trade() :Event("etrade")
	{}
	CThostFtdcTradeField trade;
};

class Event_Position : public Event
{
public:
	Event_Position() :Event("eposition")
	{}
	CThostFtdcInvestorPositionField position;
};

class Event_Account : public Event
{
public:
	Event_Account() :Event("eaccount")
	{}
	CThostFtdcTradingAccountField account;
};

class Event_Error : public Event
{
public:
	Event_Error() :Event("eerror")
	{}
	CThostFtdcRspInfoField error;
};

class Event_Log : public Event
{
public:
	Event_Log() :Event("elog")
	{}
	int req;
	std::string message;
	std::string log_datetime;
};


#endif // !COMFUC_H