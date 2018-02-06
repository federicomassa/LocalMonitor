#include "Observer.h"

#include <iostream>

Observer::Observer(const std::string& n)
{
	name = n;	
}

Observer::Observer(const Observer& obs)
{
	*this = obs;
}

Observer & Observer::operator=(const Observer& obs)
{
	name = obs.name;
	selfTrajectory = obs.selfTrajectory;
	othersTrajectory = obs.othersTrajectory;
	environmentTrajectory = obs.environmentTrajectory;

	return *this;
}

Observer::~Observer()
{
}

bool Observer::operator==(const Observer& obs) const
{
	return (name == obs.name);
}

bool Observer::operator!=(const Observer& obs) const
{
	return !(*this == obs);
}

const std::string & Observer::GetName() const
{
	return name;
}

const std::string & Observer::GetObserverID() const
{
	return observerID;
}

const std::string & Observer::GetObservedID() const
{
	return observedID;
}



// Default behaviour, store only latest measure
void Observer::ReceiveSensorOutput(const SensorOutput& sensorOutput, const double& currentTime)
{	
	const Agent& currentSelf = sensorOutput.RetrieveSelfData();
	const AgentVector& currentOthers = sensorOutput.RetrieveOtherAgentsData();
	const EnvironmentParameters& currentEnv = sensorOutput.RetrieveEnvironmentData();
	
	selfTrajectory.clear();
	othersTrajectory.clear();
	environmentTrajectory.clear();
	
	selfTrajectory.insert(currentTime, currentSelf);
	othersTrajectory.insert(currentTime, currentOthers);
	environmentTrajectory.insert(currentTime, currentEnv);
}

void Observer::SetClassName(const std::string& className)
{
	name = className;
}

void Observer::SetObservedID(const std::string& id)
{
	observedID = id;
}

void Observer::SetObserverID(const std::string& id)
{
	observerID = id;
}


// By default, it does not do anything
void Observer::Configure(const nlohmann::json& observingJson)
{}

// By default, it does not do anything
void Observer::PreConfigure(const nlohmann::json& topJson)
{}


