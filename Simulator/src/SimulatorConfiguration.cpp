#include "SimulatorConfiguration.h"
#include "Utility/LogFunctions.h"
#include "Utility/Logger.h"

#include "SimulatorBuildParams.h"

#include <iostream>
#include <math.h>
#include <set>

using namespace std;
using json = nlohmann::json;
using namespace LogFunctions;

extern Logger logger;

SimulatorConfiguration::SimulatorConfiguration(const string &fileName)
{
    ifstream file(fileName.c_str());
    file >> j;
	
	RegisterEntry("agents", MANDATORY);
	RegisterEntry("simulation_time_span", MANDATORY);
	RegisterEntry("simulation_time_step", MANDATORY);
	RegisterEntry("simulator_viewer");
	RegisterEntry("parameters");
}


void SimulatorConfiguration::Parse()
{
   

    // Parse agents
    json agentsJson = GetEntry("agents");

    json simulTimeSpanJson = GetEntry("simulation_time_span");
	simulTimeSpan = simulTimeSpanJson.get<double>();

    json simulTimeStepJson = GetEntry("simulation_time_step");
	simulTimeStep = simulTimeStepJson.get<double>();
	
    simulRealTimeSpan = simulTimeStep * ceil(simulTimeSpan / simulTimeStep);
    simulSteps = round(simulRealTimeSpan / simulTimeStep);

    set<string> IDSet;

    for (int agentIndex = 0; agentIndex < agentsJson.size(); agentIndex++) {
        json agentJson = agentsJson.at(agentIndex);

        SimulAgent a = ReadAgent(agentJson);
        agents[a.GetID()] = a;
        pair<set<string>::iterator, bool> lastID = IDSet.insert(a.GetID());
        if (lastID.second == false) {
            Error("SimulatorConfiguration::Parse", "Agents should have different IDs");
        }
    }
    
    try{
		j.at("simulator_viewer");
		useSimulatorViewer = true;
	}
	catch(std::out_of_range& e)
	{
		useSimulatorViewer = false;
	}
	
	
	json parametersJ = GetEntry("parameters");
	for (json::iterator it = parametersJ.begin(); it != parametersJ.end(); it++)
		parameters[string(it.key())] = double(it.value());
	

}

SimulAgent SimulatorConfiguration::ReadAgent(const json &agent)
{
    SimulAgent a;
	AgentParameters aParam;

    try {
		for (json::const_iterator it = agent.begin(); it != agent.end();
			 it++)
			 {
				 // List of standard entries
				if (it.key() != "id" && 
					it.key() != "state_variables" &&
					it.key() != "init_states" &&
					it.key() != "maneuvers" &&
					it.key() != "init_maneuver" &&
					it.key() != "kinematics" &&
					it.key() != "controller" &&
					it.key() != "automaton" &&
					it.key() != "visibility" &&
					it.key() != "communication")
					
					aParam[it.key()] = it.value();
			 }
		
        a.SetID(agent.at("id"));
        json stateVars = agent.at("state_variables");
        json initStates = agent.at("init_states");

        if (stateVars.size() != initStates.size()) 
		{
            Error("SimulatorConfiguration::Parse", "There should be one init_states entry for each state_variables entry in agent " + a.GetID());
        }

        StateMap stateVector;

        for (int stateVar = 0; stateVar < stateVars.size(); stateVar++) {

            stringstream ss;

            string stateVarName = stateVars.at(stateVar).get<string>();
            
            ss << initStates.at(stateVar);
            double initStateVar;
            ss >> initStateVar;
            ss.clear();

            stateVector[stateVarName] = initStateVar;

        }

        a.SetState(State(stateVector));

		json maneuverList = agent.at("maneuvers");
		ManeuverList manList;
		for (int man = 0; man < maneuverList.size(); man++)
		{
			if (!manList.insert(maneuverList.at(man).get<string>()).second)
				Error("SimulatorConfiguration::ReadAgent", "Found duplicate maneuver in agent " + a.GetID());
		}
		a.SetPossibleManeuvers(manList);
		
		string initManeuver = agent.at("init_maneuver").get<string>();
		if(!a.SetManeuver(initManeuver))
			Error("SimulatorConfiguration::ReadAgent", "In agent " + a.GetID() + ": init_maneuver was not found among the possible maneuvers");
		
        a.SetKinematics(agent.at("kinematics"));

    } 
    catch (out_of_range &e) {
        logger << "In SimulatorConfiguration::ReadAgent, mandatory entry not found --- " << e.what() << logger.EndL();
        exit(1);
	}

	agentsParameters[a.GetID()] = aParam;
	
	return a;
    
}

void SimulatorConfiguration::RegisterEntry(const string& entryName, const EntryType& type)
{
	entries.insert(Entry(entryName, type));
}

nlohmann::json SimulatorConfiguration::GetEntry(const std::string& entryName) const
{
	json entryJ;
	std::set<Entry>::const_iterator foundEntry = entries.end();
	
	for (std::set<Entry>::const_iterator entry = entries.begin(); 
		entry != entries.end(); entry++)
		 {
			if (entry->name == entryName)
			{
				foundEntry = entry;
				break;
			}
				
		 }
		 
	if (foundEntry == entries.end())
		Error("SimulatorConfiguration::FindEntry", "Forgot to register entry?");
		
	try
	{
		entryJ = j.at(entryName);
	}
	catch(std::out_of_range& e)
	{
		if (foundEntry->type == MANDATORY)
			Error("SimulatorConfiguration::FindEntry", "Entry " + entryName + " in configuration file is mandatory. Please insert it.");
	}
	
	return entryJ;
}

const SimulAgentVector &SimulatorConfiguration::GetAgents() const {
	return agents;
}

const int &SimulatorConfiguration::GetSimulationSteps() const {
	return simulSteps;
}

const double &SimulatorConfiguration::GetSimulationTimeStep() const
{
	return simulTimeStep;
}

const bool& SimulatorConfiguration::UseSimulatorViewer() const
{
	return useSimulatorViewer;
}

const SimulationParameters& SimulatorConfiguration::GetParameters() const
{
	return parameters;
}

const AgentParameters & SimulatorConfiguration::GetAgentParameters(const std::string& ID) const
{
	try
	{
		return agentsParameters.at(ID);
	}
	catch (out_of_range&)
	{
		Error("SimulatorConfiguration::GetAgentParameters", string("Agent ") + ID + " not found");
	}
}
