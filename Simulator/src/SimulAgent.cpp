#include "SimulAgent.h"
#include "SimulatorConfiguration.h"
#include "Utility/LogFunctions.h"
#include "Utility/Logger.h"

// Automatically created header during configure
// It contains all the dynamic models declared in the config file
#include "Input/Dynamics/DynamicModels.h"

#include <iostream>

using namespace std;
using namespace LogFunctions;

extern Logger logger;
extern SimulatorConfiguration conf;

SimulAgent::SimulAgent() : pLayer(conf.GetSimulationTimeStep())
{
}

SimulAgent::SimulAgent(const SimulAgent& a) : pLayer(a.pLayer)
{
	agent = a.agent;
}

const string &SimulAgent::GetID() const
{
    return agent.GetID();
}

void SimulAgent::SetID(const string& str)
{
    agent.SetID(str);
}
const State &SimulAgent::GetState() const
{
    return agent.GetState();
}

const DynamicModel& SimulAgent::GetDynamicModel() const
{
	return dynamicModel;
}

void SimulAgent::SetState(const State& q)
{
    agent.SetState(q);
}

void SimulAgent::SetPossibleManeuvers(const ManeuverList& manList)
{
	agent.SetPossibleManeuvers(manList);
}

bool SimulAgent::SetManeuver(const ManeuverName& manName)
{
	return agent.SetManeuver(manName);
}


//TODO Check compatibility between model and function
void SimulAgent::SetDynamicModel(const DynamicModel& m)
{
	pLayer.SetDynamicModel(m);
}


State SimulAgent::GenerateWorldState(const State& worldState)
{
	this->worldState = pLayer.GetDynamicModel().GetWorldState(agent, worldState);
	return (this->worldState);
}

const State& SimulAgent::GetWorldState() const
{
	return worldState;
}


const Agent& SimulAgent::GetAgent() const
{
	return agent;
}

Logger &operator<<(Logger &os, const SimulAgent &a)
{
    os << a.agent;
    return os;
}


void SimulAgent::EvolveState()
{
    agent.SetState(pLayer.GetNextState(agent.GetState(), agent.GetManeuver()));
}

void SimulAgent::SetParameters(const AgentParameters& pars)
{
	agent.SetParameters(pars);
}

std::map<std::string, ExternalSensorOutput> SimulAgent::RetrieveExternalSensorData(const std::string& sensorName, const Agent& selfInWorld, const AgentVector& othersInWorld, const EnvironmentParameters& envParams)
{
	const ExternalSensor* sensor = nullptr;
	for (auto itr = extSensors.begin(); itr != extSensors.end(); itr++)
	{
		if ((*itr)->GetName() == sensorName)
			sensor = *itr;
	}
	
	if (sensor == nullptr)
		Error("SimulAgent::RetrieveExternalSensorData", string("Sensor \'") + sensorName + "\'"  + "not found in SimulAgent sensor list");
	
	StateRegion visibleRegion;
	std::set<std::string> visibleIDs;
	
	sensor->SimulateVisibility(visibleRegion, visibleIDs, selfInWorld, othersInWorld);
	
	
}

InternalSensorOutput SimulAgent::RetrieveInternalSensorOutput(const std::string& sensorName)
{
	
}

