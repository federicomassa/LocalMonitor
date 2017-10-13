#include "SimulatorConfiguration.h"
#include "Utility/LogFunctions.h"
#include "Utility/Logger.h"

#include <iostream>
#include <math.h>
#include <set>

using namespace std;
using json = nlohmann::json;
using namespace LogFunctions;

extern Logger logger;

SimulatorConfiguration::SimulatorConfiguration(const string &fileName)
{
    file.open(fileName.c_str());
}


void SimulatorConfiguration::Parse()
{
    json j;
    file >> j;

    // Parse agents
    json agentsJson;

    json simulTimeSpanJson;

    try {
        simulTimeSpanJson = j.at("simulation_time_span");
        simulTimeSpan = simulTimeSpanJson.get<double>();

        if (simulTimeSpan <= 0) {
            Error("SimulatorConfiguration::Parse", "\"simulation_time_span\" entry should be a positive real number");
        }
    } catch (out_of_range &e) {
        Error("SimulatorConfiguration::Parse", "\"simulation_time_span\" entry in config file is mandatory");
    }

    json simulTimeStepJson;

    try {
        simulTimeStepJson = j.at("simulation_time_step");
        simulTimeStep = simulTimeStepJson.get<double>();


        if (simulTimeStep <= 0) {
            Error("SimulatorConfiguration::Parse", "\"simulation_time_step\" entry should be a positive real number");
        }

    } catch (out_of_range &e) {
        Error("SimulatorConfiguration::Parse", "\"simulation_time_step\" entry in config file is mandatory");
    }

    simulRealTimeSpan = simulTimeStep * ceil(simulTimeSpan / simulTimeStep);
    simulSteps = round(simulRealTimeSpan / simulTimeStep);

    try {
        agentsJson = j.at("agents");
    } catch (out_of_range &e) {
        Error("SimulatorConfiguration::Parse", "\"agents\" entry in config file is mandatory");
    }

    set<string> IDSet;
    
    for (int agentIndex = 0; agentIndex < agentsJson.size(); agentIndex++) {
        json agentJson = agentsJson.at(agentIndex);

        SimulAgent a = ReadAgent(agentJson);
        agents.push_back(a);
		pair<set<string>::iterator, bool> lastID = IDSet.insert(a.GetID());
		if (lastID.second == false)
		{
			Error("SimulatorConfiguration::Parse", "Agents should have different IDs");
		}
    }

}

SimulAgent SimulatorConfiguration::ReadAgent(const json &agent)
{
    SimulAgent a;

    try {
        a.SetID(agent.at("id"));
        json stateVars = agent.at("state_variables");
        json initStates = agent.at("init_states");

        if (stateVars.size() != initStates.size()) {
            Error("SimulatorConfiguration::Parse", "There should be one init_states entry for each state_variables entry in agent " + a.GetID());
        }

        StatePairVector stateVector;

        for (int stateVar = 0; stateVar < stateVars.size(); stateVar++) {

            stringstream ss;

            ss << stateVars.at(stateVar);
            string stateVarName;
            ss >> stateVarName;
            ss.clear();

            ss << initStates.at(stateVar);
            double initStateVar;
            ss >> initStateVar;
            ss.clear();

            StatePair pair = make_pair(stateVarName, initStateVar);

            stateVector.push_back(pair);
        }

        a.SetState(State(stateVector));
    } catch (out_of_range &e) {
        cerr << e.what() << endl;
        exit(1);
    }

    return a;
}

const SimulAgentVector &SimulatorConfiguration::GetAgents() const
{
    return agents;
}

const int &SimulatorConfiguration::GetSimulationSteps() const
{
    return simulSteps;
}


