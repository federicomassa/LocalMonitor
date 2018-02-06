#include "LocalMonitor.h"
#include "Utility/LogFunctions.h"
#include "Observer.h"
#include "Input/Observers/Observers.h"
#include "Automation/SensorOutput.h"

#include <fstream>

using namespace nlohmann;
using namespace std;
using namespace LogFunctions;

LocalMonitor::LocalMonitor()
{
}

LocalMonitor::~LocalMonitor()
{
	for (auto itr = observers.begin(); itr != observers.end(); itr++)
		delete (*itr);
}


void LocalMonitor::Configure(const std::string& myID, const std::string& configFilePath)
{
	observerID = myID;
	
	ifstream configFile(configFilePath.c_str());
	
	if (!configFile.good())
	{
		Info("LocalMonitor::Configure", string("Observer not set because ") + configFilePath + " was not found.");
		return;
	}
	
	try
	{
		configFile >> j;
	}
	catch(std::invalid_argument& e)
	{
		Error("LocalMonitor::Configure", string(e.what()));
	}
	
	// ========= CHECK MANDATORY ENTRIES ============
	std::vector<std::string> mandatoryEntries;
	mandatoryEntries.push_back("observing");
	
	for (auto itr = mandatoryEntries.begin();  itr != mandatoryEntries.end(); itr++)
	{
		try
		{
			j.at(*itr);
		}
		catch(out_of_range&)
		{
			Error("LocaLMonitor::Configure", string("Mandatory entry ") + *itr + " not found in observer configuration file " + configFilePath); 
		}
	}
	// =============================================
	
	
	json observingJson = j.at("observing");
	if (!observingJson.is_array())
	{
		Error("LocalMonitor::Configure", string("observing entry should be an array in  ") + configFilePath);
	}
	
	for (auto itr = observingJson.begin(); itr != observingJson.end(); itr++)
	{
		ReadObserving(*itr);
	}
	
}

void LocalMonitor::ReadObserving(const basic_json<>& observingJson)
{
	// ========= CHECK MANDATORY ENTRIES ============
	std::vector<std::string> mandatoryEntries;
	mandatoryEntries.push_back("id");
	mandatoryEntries.push_back("type");
	
	for (auto itr = mandatoryEntries.begin();  itr != mandatoryEntries.end(); itr++)
	{
		try
		{
			observingJson.at(*itr);
		}
		catch(out_of_range&)
		{
			Error("LocaLMonitor::ReadObserving", string("Mandatory entry ") + *itr + " not found in observer configuration file, inside observing entry"); 
		}
	}
	// =============================================
	
	const string observedID = observingJson.at("id").get<string>();
	const string observerType = observingJson.at("type").get<string>();

	Observer* observer = InstantiateObserver(observerType);
	observer->SetClassName(observerType);
	observer->SetObservedID(observedID);
	observer->SetObserverID(observerID);
	observer->PreConfigure(j);
	observer->Configure(observingJson);
	
	observers.push_back(observer);
}

void LocalMonitor::ReceiveSensorOutput(const SensorOutput& output, const double& currentTime)
{
	for (auto itr = observers.begin(); itr != observers.end(); itr++)
		(*itr)->ReceiveSensorOutput(output, currentTime);
}

void LocalMonitor::Run(const double& currentTime)
{
	for (auto itr = observers.begin(); itr != observers.end(); itr++)
		(*itr)->Run(currentTime);
}



