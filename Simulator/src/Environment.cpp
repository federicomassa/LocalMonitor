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
    invisibleRegionFcn = 0;
}

void Environment::Configure(const SimulatorConfiguration &conf)
{
	if (agents.size() != 0)
		Error("Environment::Configure", "Agent vector not empty");
	
    for (SimulAgentVector::const_iterator itr = conf.GetAgents().begin();
		 itr != conf.GetAgents().end(); itr++)
		 {
			agents.push_back(*itr);
		 }
	
}

const SimulAgentVector &Environment::GetAgents() const
{
    return agents;
}

void Environment::Run()
{
	logger << "Time: " << currentTimeStep << logger.EndL();
	
    for (SimulAgentVector::iterator agent = agents.begin(); agent != agents.end(); agent++) {
		logger << (*agent) << logger.EndL();
        agent->EvolveState();
    }
}

StateRegion Environment::InvisibleRegion(Agent myAgent, AgentVector otherAgents)
{
    if (invisibleRegionFcn == 0) {
        Error("Environment::InvisibleRegion", "No visibility function found");
    }

    return (*invisibleRegionFcn)(myAgent, otherAgents);
}
 
