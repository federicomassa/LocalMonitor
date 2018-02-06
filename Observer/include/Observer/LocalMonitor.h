#ifndef LOCAL_MONITOR_H
#define LOCAL_MONITOR_H

#include <vector>
#include <string>
#include <map>
#include "json.hpp"

class Observer;
class SensorOutput;

// LocalMonitor is an optional (and special) agent component that defines the observers for each external agent. It is configured by a separate configuration file for each monitoring agent, because LocalMonitor should also be able to run by itself. The simulator is to be thought as a simulating platform for the LocalMonitor 
class LocalMonitor
{
	std::vector<Observer*> observers;
	nlohmann::json j;
	std::string observerID;
	void ReadObserving(const nlohmann::json&);
public:
	LocalMonitor();
	~LocalMonitor();
	void Configure(const std::string& myID, const std::string& configFilePath);
	void ReceiveSensorOutput(const SensorOutput&, const double& currentTime);
	void Run(const double& currentTime);
};

#endif
