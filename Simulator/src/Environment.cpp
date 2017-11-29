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
	typedef map<string, State> WorldStates;
	
	// ========= Fill states of agents (in world coordinates) ==========
	WorldStates worldStates;
	for (auto agent = agents.begin(); agent != agents.end(); agent++)
	{
		const string& ID = agent->first;
		const State& state = agent->second.GetWorldState();
		
		worldStates[ID] = state;
	}
	
    for (SimulAgentVector::iterator agent = agents.begin(); agent != agents.end(); agent++) 
	{
		const State& self = worldStates.at(agent->first);
		SimulAgent& selfAgent = agent->second;
		
		// Fill with states of agents different than self
		WorldStates neighStates;
		for (auto itr = worldStates.begin(); itr != worldStates.end(); itr++)
			if (itr->first != agent->first)
				neighStates[itr->first] = itr->second;
		
		selfAgent.RetrieveSensorData();
			
		selfAgent.EvolveState();
	}
}

void Environment::ConvertAgentsToWorld()
{
	for (auto agent = agents.begin(); agent != agents.end(); agent++)
	{
		SimulAgent& a = agent->second;
		State worldState = a.GenerateWorldState(GenerateWorldAgentState());
	}
}


State Environment::GenerateWorldAgentState() const
{
	State worldAgentState;
	
	for (auto itr = agentFeatures.begin(); itr != agentFeatures.end(); itr++)
		worldAgentState.AddStateVariable(*itr);
	
	return worldAgentState;
}

State Environment::GenerateWorldEnvironmentState() const
{
	State worldEnvState;
	
	for (auto itr = envFeatures.begin(); itr != envFeatures.end(); itr++)
		worldEnvState.AddStateVariable(*itr);
	
	return worldEnvState;
}

