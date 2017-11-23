#include "Environment.h"
#include "Utility/Logger.h"
#include "Utility/LogFunctions.h"
#include "Utility/SystemTypes.h"

#include <iostream>

using namespace std;
using namespace LogFunctions;

extern Logger logger;
extern int currentTimeStep;

Environment::Environment()
{
}

void Environment::Configure(const SimulatorConfiguration &conf)
{
	if (agents.size() != 0)
		Error("Environment::Configure", "Agent vector not empty");
	
    for (SimulAgentVector::const_iterator itr = conf.GetAgents().begin();
		 itr != conf.GetAgents().end(); itr++)
		 {
			agents[itr->first] = itr->second;
		 }
		 
	agentFeatures = conf.GetWorldAgentFeatures();
	envFeatures = conf.GetWorldEnvironmentFeatures();
	
}

const SimulAgentVector &Environment::GetAgents() const
{
    return agents;
}

void Environment::Run()
{	
    for (SimulAgentVector::iterator agent = agents.begin(); agent != agents.end(); agent++) {
		agent->second.EvolveState();
    }
}

void Environment::ConvertAgentsToWorld(const SimulatorConfiguration& conf)
{
	for (auto agent = agents.begin(); agent != agents.end(); agent++)
	{
		SimulAgent& a = agent->second;
		
		
	}
}
