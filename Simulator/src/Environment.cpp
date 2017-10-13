#include "Environment.h"
#include "Utility/Logger.h"
#include "Utility/LogFunctions.h"
#include "Utility/SystemTypes.h"

#include <iostream>

using namespace std;
using namespace LogFunctions;

extern Logger logger;

Environment::Environment()
{
	invisibleRegionFcn = 0;
}

void Environment::Configure(const SimulatorConfiguration &conf)
{
    agents = conf.GetAgents();
}

const SimulAgentVector &Environment::GetAgents() const
{
    return agents;
}

void Environment::Run() 
{
	for (SimulAgentVector::iterator agent = agents.begin(); agent != agents.end(); agent++)
	{
		agent->EvolveState();
	}
}

StateRegion Environment::InvisibleRegion(Agent myAgent, AgentVector otherAgents)
{
	if (invisibleRegionFcn == 0)
		Error("Environment::InvisibleRegion", "No visibility function found");
	
	return (*invisibleRegionFcn)(myAgent, otherAgents);
}
