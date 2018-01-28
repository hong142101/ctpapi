#include "API.h"
#include <fstream>

/////////////////////////////////////////////////////////////////////////
CTPAPI::CTPAPI(EventEngine *eventengine)// : API::API(eventengine)
{
	this->mdapi = new MdApi(this);
	this->tdapi = new TdApi(this);
	//this->tdapi = new TdApi;
	//this->tdapi->ctpapi = this;
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
	while (!this->tdapi->get_is_instrument_finished())
	{ }
	//for (auto id = this->tdapi->InstrumentID->begin(); id != this->tdapi->InstrumentID->end(); ++id)
	//	char *ppInstrumentID[1] =
	//	this->mdapi->SubscribeMarketData(id, 1);
	//char** contract = new char *[this->tdapi->InstrumentID->size()];
	//char* contract[1000] = &this->tdapi->InstrumentID->front();
	//std::printf("sizeof(InstrumentID): %zi\n", this->tdapi->InstrumentID.size());
	//std::cout << "InstrumentID: " << *this->tdapi->InstrumentID.data() << std::endl;
	//this->mdapi->SubscribeMarketData((char**)this->tdapi->InstrumentID.data(), (int)this->tdapi->InstrumentID.size());

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

//class TickRecords
//{
//private:
//	std::map<std::string, CThostFtdcInstrumentField*> instrument_info;
//public:
//	void input_instrument_info(const std::map<std::string, CThostFtdcInstrumentField*> instrument_info)
//	{
//		this->instrument_info = instrument_info;
//	}
//
//	void tick_records(std::shared_ptr<Event>e)
//	{
//		std::shared_ptr<Event_Tick> etick = std::static_pointer_cast<Event_Tick>(e);
//		std::thread::id this_id = std::this_thread::get_id();
//		//std::cout << etick->GetEventType() << std::endl;
//		std::string tick = "Tick";
//		std::string contract = etick->tick.InstrumentID;
//		std::string product = this->instrument_info[contract]->ProductID;
//		std::string future = "Future";
//		std::string exchange = this->instrument_info[contract]->ExchangeID;
//		std::string head = "China";
//		std::string root = "tdb_research";
//		std::string contract_folder_path = "./" + root + "/" + head + "/" + exchange + "/" + future + "/" + product + "/" + contract + "/" + tick;
//		std::string filename = contract + "_" + exchange + "_tick.csv";
//		std::string file_path = contract_folder_path + "/" + filename;
//		if (CommonFuction::folder_not_exists_and_create(contract_folder_path))
//		{
//			std::fstream file;
//			if (!CommonFuction::file_if_exists(file_path))
//			{
//				while (!file.is_open())
//				{
//					file.open(file_path, std::fstream::ios_base::app);
//				}
//				file << "instrument,date,time,millisec,upper_limit_price,lower_limit_price,"
//					"open,high,low,last_price,volume,open_int,"
//					"ask_price1,ask_vol1,ask_price2,ask_vol2,ask_price3,ask_vol3,ask_price4,ask_vol4,ask_price5,ask_vol5,"
//					"ask_price6,ask_vol6,ask_price7,ask_vol7,ask_price8,ask_vol8,ask_price9,ask_vol9,ask_price10,ask_vol10,"
//					"bid_price1,bid_vol1,bid_price2,bid_vol2,bid_price3,bid_vol3,bid_price4,bid_vol4,bid_price5,bid_vol5,"
//					"bid_price6,bid_vol6,bid_price7,bid_vol7,bid_price8,bid_vol8,bid_price9,bid_vol9,bid_price10,bid_vol10,"
//					"settle_price,pre_settle_price"
//					<< std::endl;
//				file.close();
//			}
//			while (!file.is_open())
//			{
//				file.open(file_path, std::fstream::ios_base::app);
//			}
//			file << etick->tick.ActionDay
//				<< ","
//				<< etick->tick.TradingDay
//				<< ","
//				<< etick->tick.UpdateTime
//				<< "."
//				<< etick->tick.UpdateMillisec
//				<< std::endl;
//			file.close();
//		}
//	}
//};
//
//int main()
//{
//	EventEngine eventengine;
//	CTPAPI* ctpapi = new CTPAPI(&eventengine);
//	TickRecords tr;
//	const std::function<void(std::shared_ptr<Event>)> f = std::bind(&TickRecords::tick_records, &tr, std::placeholders::_1);
//	ctpapi->ctp_connect();
//
//	const std::vector<std::string> *all_InstrumentID = ctpapi->get_all_InstrumentID();
//	const std::map<std::string, CThostFtdcInstrumentField*> *instrument_info = ctpapi->get_instrument_info();
//	tr.input_instrument_info(*instrument_info);
//
//	for (auto id = all_InstrumentID->begin(); id != all_InstrumentID->end(); ++id)
//	{
//		eventengine.regEvent("etick" + *id, f);
//	}
//	eventengine.startEngine();
//	ctpapi->join();
//}
