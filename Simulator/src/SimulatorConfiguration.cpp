#include "SimulatorConfiguration.h"
#include "Utility/LogFunctions.h"
#include "Utility/Logger.h"

#include "SimulatorViewer.h"
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
        if (lastID.second == false) {
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

	return a;
    
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


