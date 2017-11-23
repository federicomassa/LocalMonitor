#include "SimulAgent.h"
#include "SimulatorConfiguration.h"
#include "Utility/LogFunctions.h"
#include "Utility/Logger.h"

// Automatically created header during configure
// It contains all the dynamic models declared in the config file
#include "Input/Dynamics/DynamicModels.h"

#include <iostream>

using namespace std;

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

void SimulAgent::SetDynamicFunction(const string& fcnName)
{
    pLayer.SetDynamics(fcnName);
}

//TODO Check compatibility between model and function
void SimulAgent::SetDynamicModel(const DynamicModel& m)
{
	pLayer.SetDynamicModel(m);
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

