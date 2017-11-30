#include "Environment.h"
#include "Utility/Logger.h"
#include "Utility/LogFunctions.h"
#include "Utility/SystemTypes.h"

#include "Automation/SensorOutput.h"

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
	envParameters = conf.GetEnvironmentParameters();
}

const SimulAgentVector &Environment::GetAgents() const
{
    return agents;
}

void Environment::Run()
{	
	/*
	// ========= Fill states of agents (in world coordinates) ==========
	for (auto agent = agents.begin(); agent != agents.end(); agent++)
	{
		const string& ID = agent->first;
		const State& state = agent->second.GetWorldState();
		
		worldStates[ID] = state;
	}
	*/
	
    for (SimulAgentVector::iterator agent = agents.begin(); agent != agents.end(); agent++) 
	{
		SimulAgent& selfAgent = agent->second;
		
		Agent selfWorld;
		selfWorld.SetID(agent->first);
		selfWorld.SetState(selfAgent.GetWorldState());
		
		// Fill with states of agents different than self
		AgentVector othersInWorld;
		for (auto otherAgent = agents.begin(); otherAgent != agents.end(); otherAgent++)
			if (otherAgent->first != agent->first)
			{
				// Build world agent
				Agent otherWorldAgent;
				otherWorldAgent.SetID(otherAgent->first);
				otherWorldAgent.SetState(otherAgent->second.GetWorldState());
				othersInWorld[otherAgent->first] = otherWorldAgent;
			}
		
		SensorOutput sensorOutput = selfAgent.SimulateSensors(selfWorld, othersInWorld, envParameters);
			
		selfAgent.EvolveState(sensorOutput);
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

