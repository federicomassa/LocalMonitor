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

//FIXME Maybe it's better to only define mandatory entries, via RegisterMandatoryEntry,
// and make GetEntry iterate on the whole json, so that user defined properties are
// more easily found by other modules (such as Viewer). 
// Example: HighwayViewer needs a subject_id. The user writes in the config file
// "subject_id" : "1" 

SimulatorConfiguration::SimulatorConfiguration(const string &fileName)
{
    ifstream file(fileName.c_str());
    file >> j;
	
	RegisterMandatoryEntry("agents");
	RegisterMandatoryEntry("simulation_time_span");
	RegisterMandatoryEntry("simulation_time_step");
	RegisterMandatoryEntry("dynamic_models");

	RegisterStandardEntry("simulator_viewer");
	RegisterStandardEntry("world_environment_features");
	RegisterStandardEntry("world_agent_features");
}


void SimulatorConfiguration::Parse()
{
   // First check that all mandatory entries are present
	for (auto itr = mandatoryEntries.begin(); itr != mandatoryEntries.end(); itr++)
	{
		try
		{
			json mandatoryJ = GetEntry(*itr);
		}
		catch (out_of_range&)
		{
			Error("SimulatorConfiguration::Parse", string("Mandatory entry ") + (*itr) + " not found");
		}
	}
	
	
	// Check for custom entries
	for (auto itr = j.begin(); itr != j.end(); itr++)
	{
		if (standardEntries.find(itr.key()) == standardEntries.end())
			parameters[itr.key()] = itr.value();
	}
	
	json dynModelsJson = GetEntry("dynamic_models");
	for (int modelIndex = 0; modelIndex < dynModelsJson.size(); modelIndex++)
	{
		AddDynamicModel(dynModelsJson.at(modelIndex));
	}
	
	
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
    
    try
    {
		GetEntry("simulator_viewer");
		useSimulatorViewer = true;
	}
	catch(std::out_of_range& e)
	{
		useSimulatorViewer = false;
	}
	
	
	//Optional, world features
	try
	{
		json envFeaturesJson = GetEntry("world_environment_features");
		for (json::iterator it = envFeaturesJson.begin(); it != envFeaturesJson.end(); it++)
			envFeatures[string(it.key())] = it.value().get<double>();
	}
	catch (out_of_range&)
	{}
	
	try
	{
		//TODO check that user has inserted an array of string
		json agentFeaturesJson = GetEntry("world_agent_features");
		for (int index = 0; index < agentFeaturesJson.size(); index++)
		{
			agentFeatures.insert(agentFeaturesJson.at(index).get<string>());
		}
	}
	catch (out_of_range&)
	{}

}

// TODO Manage mandatory agents entries
SimulAgent SimulatorConfiguration::ReadAgent(const json &agent)
{
    SimulAgent a;
	AgentParameters aCustomEntries;

    try {
		for (json::const_iterator it = agent.begin(); it != agent.end();
			 it++)
			 {
				 // List of standard entries. Anything different than these is regarded as an agent parameter (used for example to set image path for viewer)
				if (it.key() != "id" && 
					it.key() != "init_states" &&
					it.key() != "init_maneuver" &&
					it.key() != "visibility" &&
					it.key() != "communication" &&
					it.key() != "sensing" &&
					it.key() != "parameters")
					aCustomEntries[it.key()] = it.value();
			 }
		
        a.SetID(agent.at("id"));
		
		string dynamicModelName = agent.at("dynamic_model").get<string>();
		DynamicModel tmpModel;
		tmpModel.SetName(dynamicModelName);
		
		auto dynItr = dynamicModels.find(tmpModel);
		if (dynItr == dynamicModels.end())
			Error("SimulatorConfiguration::ReadAgent", string("Dynamic model ") + dynamicModelName + " of agent " + a.GetID());
		
		// If model was found, set it for current agent
		a.SetDynamicModel(*dynItr);
		
        json initStates = agent.at("init_states");
		const int varSize = a.GetDynamicModel().GetStateVariables().size();
		
        if (varSize != initStates.size()) 
		{
            Error("SimulatorConfiguration::ReadAgent", "There should be one init_states entry for each state_variables entry in agent " + a.GetID());
        }

        StateMap stateVector;

        for (auto stateItr = a.GetDynamicModel().GetStateVariables().begin(); 
			 stateItr != a.GetDynamicModel().GetStateVariables().end(); stateItr++)
				stateVector[*stateItr] = initStates.at(*stateItr).get<double>();

        a.SetState(State(stateVector));

		
		// Parameters entry is optional in agent
		try
		{
			json agentParameters = agent.at("parameters");
			AgentParameters aParam;
			for (auto parItr = agentParameters.begin(); 
				 parItr != agentParameters.end(); parItr++)
					aParam[parItr.key()] = parItr.value();
					
			a.SetParameters(aParam);
				 
		}
		catch(out_of_range&)
		{}
		
    }
    
    catch (out_of_range &e) {
        logger << "In SimulatorConfiguration::ReadAgent, mandatory entry not found --- " << e.what() << logger.EndL();
        exit(1);
	}

	agentsCustomEntries[a.GetID()] = aCustomEntries;
	
	//Sensing s = ReadSensing(agent);
	//a.SetSensing(s);
	
	return a;
    
}

/*
Sensing SimulatorConfiguration::ReadSensing(const nlohmann::json& agent)
{
	
}
*/

void SimulatorConfiguration::RegisterMandatoryEntry(const string& entryName)
{
	mandatoryEntries.insert(entryName);
	standardEntries.insert(entryName);
}

void SimulatorConfiguration::RegisterStandardEntry(const string& entryName)
{
	standardEntries.insert(entryName);
}

// Throws out_of_range exception
nlohmann::json SimulatorConfiguration::GetEntry(const std::string& entryName) const
{
	return j.at(entryName);
}

// Throws out_of_range exception
nlohmann::json SimulatorConfiguration::GetEntry(const std::string& entryName, const nlohmann::json& jFile)
{
	return jFile.at(entryName);
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

const AgentParameters & SimulatorConfiguration::GetAgentCustomEntry(const std::string& ID) const
{
	try
	{
		return agentsCustomEntries.at(ID);
	}
	catch (out_of_range&)
	{
		Error("SimulatorConfiguration::GetAgentParameters", string("Agent ") + ID + " not found");
	}
}

void SimulatorConfiguration::AddDynamicModel ( const nlohmann::json & modelJ)
{
	DynamicModel m;
	
	bool isNameSet = false, isStateVarSet = false, isDynamicsFunctionSet = false, 
	isControlVarSet = false, isStateConversionFunctionSet = false;
	
	for (auto itr = modelJ.begin(); itr != modelJ.end(); itr++)
	{
		if (itr.key() == "name")
		{
			m.SetName(itr.value().get<string>());
			isNameSet = true;
		}
		else if (itr.key() == "state_variables")
		{
			json stateVarsJson = itr.value();
			if (stateVarsJson.size() < 1)
				Error("SimulatorConfiguration::AddDynamicModel", "state_variables entry in dynamic_models must contain an array [] of state variable names");
			
			vector<string> stateVars;
			for (int stateIndex = 0; 
				 stateIndex < stateVarsJson.size(); stateIndex++)
					stateVars.push_back(stateVarsJson.at(stateIndex).get<string>());
			
			m.SetStateVariables(stateVars);
			isStateVarSet = true;
		}
		else if (itr.key() == "dynamics")
		{
			m.SetDynamicsFunction(itr.value().get<string>());
			isDynamicsFunctionSet = true;
		}
		else if (itr.key() == "control_variables")
		{
			json controlVarsJson = itr.value();
			if (controlVarsJson.size() < 1)
				Error("SimulatorConfiguration::AddDynamicModel", "control_variables entry in dynamic_models must contain an array [] of control variable names");
			
			vector<string> controlVars;
			for (int controlIndex = 0; 
				 controlIndex < controlVarsJson.size(); controlIndex++)
					controlVars.push_back(controlVarsJson.at(controlIndex).get<string>());
			
			m.SetControlVariables(controlVars);
			isControlVarSet = true;
		}
		else if (itr.key() == "world_conversion_function")
		{
			m.SetStateConversionFunction(itr.value().get<string>());
			isStateConversionFunctionSet = true;
		}
		else
			Error("SimulatorConfiguration::AddDynamicModel", "Unknown entry. Dynamic models must each contain a name, a set of state variables, a dynamics function name, and a set of control variables");
	}
	
	if (!isNameSet || !isStateVarSet || !isDynamicsFunctionSet || !isControlVarSet || !isStateConversionFunctionSet)
		Error("SimulatorConfiguration::AddDynamicModel", "Missing entry. Dynamic models must each contain a name, a set of state variables, a dynamics function name, a set of control variables, and a conversion function from states to world coordinates");
	
	dynamicModels.insert(m);
	
}

const WorldAgentFeatures & SimulatorConfiguration::GetWorldAgentFeatures() const
{
	return agentFeatures;
}

const WorldEnvironmentFeatures & SimulatorConfiguration::GetWorldEnvironmentFeatures() const
{
	return envFeatures;
}


