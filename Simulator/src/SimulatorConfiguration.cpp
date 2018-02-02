#include "SimulatorConfiguration.h"
#include "Utility/LogFunctions.h"
#include "Utility/MyLogger.h"
#include "Utility/Math.h"
#include "Automation/ControlModel.h"
#include "SimulatorBuildParams.h"

#include "Input/Automation/Controllers/Controllers.h"
#include "Input/Automation/Automatons/Automatons.h"
#include "Input/Dynamics/DynamicModels.h"
#include "Input/Dynamics/StateConversions.h"
#include "Input/Automation/Sensors/Sensors.h"
#include "Input/Automation/Controllers/ControllerFailures.h"

#include <iostream>
#include <math.h>
#include <set>

using namespace std;
using json = nlohmann::json;
using namespace LogFunctions;
using namespace Utility;

//FIXME Maybe it's better to only define mandatory entries, via RegisterMandatoryEntry,
// and make GetEntry iterate on the whole json, so that user defined properties are
// more easily found by other modules (such as Viewer). 
// Example: HighwayViewer needs a subject_id. The user writes in the config file
// "subject_id" : "1" 

SimulatorConfiguration::SimulatorConfiguration(const string &fileName, MyLogger* log)
{
	logger = log;
	
    ifstream file(fileName.c_str());
    file >> j;
	
	RegisterMandatoryEntry("agents");
	RegisterMandatoryEntry("simulation_time_span");
	RegisterMandatoryEntry("simulation_time_step");
	RegisterMandatoryEntry("dynamic_models");
	RegisterMandatoryEntry("control_models");
	
	RegisterStandardEntry("simulator_viewer");
	RegisterStandardEntry("world_environment_features");
	RegisterStandardEntry("world_agent_features");
	RegisterStandardEntry("sensors");
	RegisterStandardEntry("parameters");
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
			simCustomEntries.AddEntry(itr.key(), itr.value());
	}
	
	json dynModelsJson = GetEntry("dynamic_models");
	for (auto modelIndex = dynModelsJson.begin(); modelIndex != dynModelsJson.end(); modelIndex++)
	{
		AddDynamicModel(*modelIndex);
	}
	
	json controlModelsJson = GetEntry("control_models");
	for (auto modelIndex = controlModelsJson.begin(); modelIndex != controlModelsJson.end(); modelIndex++)
		AddControlModel(*modelIndex);
	
	
	
	//Optional parameters
	try
	{
		json envParametersJson = GetEntry("parameters");
		if (!envParametersJson.is_array())
			Error("SimulatorConfiguration::Parse", "Entry \'parameters\' must be an array of strings");
		
		for (auto index = envParametersJson.begin(); index != envParametersJson.end(); index++)
		{
			// Look for the parameters in the main json object
			string key = index->get<string>();
			string valueStr;
			
			try
			{
				if (GetEntry(key).is_string())
					valueStr = GetEntry(key).get<string>();
				else
					Error("SimulatorConfiguration::Parse", string("All parameters are strings, i.e. between quotes. Please define \'") + key + "\' as string");
			}
			catch(out_of_range&)
			{
				Error("SimulatorConfiguration::Parse", string("parameter \'") +
				key + "\' was not found in config file. Please set a value for it somewhere in the file");
			}
			
			envParameters.AddEntry(key, ToDouble(valueStr));
		}
	}
	catch (out_of_range&)
	{}

    json simulTimeSpanJson = GetEntry("simulation_time_span");
	simulTimeSpan = simulTimeSpanJson.get<double>();

	// FIXME This is an important parameter. For the discretized simulated system
	// to reflect the properties of the continuous system it must be sufficiently
	// small. From a calculation I did, I found out a form of the Nyquist 
	// sampling theorem: deltaT < |2/lambda_max|, where lambda_max is the value
	// of the largest (in module) eigenvalue of the continuous linearized matrix
    json simulTimeStepJson = GetEntry("simulation_time_step");
	simulTimeStep = simulTimeStepJson.get<double>();
	
    simulRealTimeSpan = simulTimeStep * ceil(simulTimeSpan / simulTimeStep);
    simulSteps = round(simulRealTimeSpan / simulTimeStep);

    
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
		for (auto index = envFeaturesJson.begin(); index != envFeaturesJson.end(); index++)
			envFeatures.insert(index->get<string>());
	}
	catch (out_of_range&)
	{}
	
	try
	{
		//TODO check that user has inserted an array of string
		json agentFeaturesJson = GetEntry("world_agent_features");
		for (auto index = agentFeaturesJson.begin(); index != agentFeaturesJson.end(); index++)
		{
			agentFeatures.insert(index->get<string>());
		}
	}
	catch (out_of_range&)
	{}

	
	try
	{
		json sensorsJson = GetEntry("sensors");
		for (auto index = sensorsJson.begin(); index != sensorsJson.end(); index++)
			ReadSensor(*index);
		
	}
	catch(out_of_range&)
	{}
		
		
	// At last, read agents
	try
	{
		set<string> IDSet;
		json agentsJson = GetEntry("agents");
		if (!agentsJson.is_array())
		Error("SimulatorConfiguration", "Entry \'agents\' must be an array");
	
		for (auto agentIndex = agentsJson.begin(); agentIndex != agentsJson.end(); agentIndex++) {
			json agentJson = *agentIndex;

			SimulAgent a = ReadAgent(agentJson);
			agents[a.GetID()] = a;
			pair<set<string>::iterator, bool> lastID = IDSet.insert(a.GetID());
			if (lastID.second == false) {
				Error("SimulatorConfiguration::Parse", "Agents should have different IDs");
			}
		}

	}
	catch (out_of_range&)
	{
		Error("SimulatorConfiguration::Parse", "\'agents\' entry is mandatory");
	}
	
}
	
// TODO Manage mandatory agents entries
SimulAgent SimulatorConfiguration::ReadAgent(const json &agent)
{
    SimulAgent a(this);
	AgentCustomEntries aCustomEntries;

    try {
		for (json::const_iterator it = agent.begin(); it != agent.end();
			 it++)
			 {
				 // List of standard entries. Anything different than these is regarded as an agent parameter (used for example to set image path for viewer)
				if (it.key() != "id" && 
					it.key() != "init_states" &&
					it.key() != "init_maneuver" &&
					it.key() != "dynamic_model" &&
					it.key() != "control_model" &&
					it.key() != "communication" &&
					it.key() != "sensors" &&
					it.key() != "parameters" &&
					it.key() != "control_failures")
					aCustomEntries.AddEntry(it.key(), it.value());
			 }
		
        a.SetID(agent.at("id"));
		agentsCustomEntries[a.GetID()] = aCustomEntries;

		string dynamicModelName = agent.at("dynamic_model").get<string>();
		DynamicModel tmpModel;
		tmpModel.SetName(dynamicModelName);
		
		auto dynItr = dynamicModels.find(tmpModel);
		if (dynItr == dynamicModels.end())
			Error("SimulatorConfiguration::ReadAgent", string("Dynamic model ") + dynamicModelName + " of agent " + a.GetID() + " was not found.");
		
		// If model was found, set it for current agent
		a.SetDynamicModel(*dynItr);
		
        json initStates = agent.at("init_states");
		const unsigned int varSize = a.GetDynamicModel().GetStateVariables().size();
		
        if (varSize != initStates.size()) 
		{
            Error("SimulatorConfiguration::ReadAgent", "There should be one init_states entry for each state_variables entry in agent " + a.GetID());
        }
        
        // NB Do this after setting dynamic model!
        // Set init states in the same order as appearing in the dynamic model
        StateMap stateVector;
        for (auto stateItr = a.GetDynamicModel().GetStateVariables().begin(); stateItr != a.GetDynamicModel().GetStateVariables().end(); stateItr++)
		{
			try
			{
				stateVector[*stateItr] = initStates.at(*stateItr).get<double>();
			}
			catch(out_of_range&)
			{
				Error("SimulatorConfiguration::ReadAgent", string("\'init_states\'") + " in agent " + a.GetID() + " does not initialize each variable of its dynamic model");
			}
		}
        a.SetState(State(stateVector));

		
		// Parameters entry is optional in agent
		try
		{
			json agentParameters = agent.at("parameters");
			if (!agentParameters.is_array())
				Error("SimulatorConfiguration::ReadAgent", "Entry \'parameters\' in agent must be an array");
			
			AgentParameters aParam;
			for (auto parIndex = agentParameters.begin(); parIndex != agentParameters.end(); 
				 parIndex++)
				 {
					string key = parIndex->get<string>();
					string value;
					// Now check if this key is linked to a value in agent json
					try
					{
						value = GetEntry(key, agent).get<string>();
					}
					catch(out_of_range&)
					{
						Error("SimulatorConfiguration::ReadAgent", string("parameter \'") +
				key + "\' was not found in agent object. Please set a value for it somewhere in the agent object.");
					}
					
					aParam.AddEntry(key, ToDouble(value));
				 }
					
			a.SetParameters(aParam);
				 
		}
		catch(out_of_range&)
		{}
		
    }
    
    catch (out_of_range &e) {
		if (logger)
			(*logger) << "In SimulatorConfiguration::ReadAgent, mandatory entry not found --- " << e.what() << logger->EndL();
        exit(1);
	}

	
	// Add sensors to agent (optional). NB Instantiate functions allocate new memory, ownership is of agent
	try
	{
		json sensorsJson = GetEntry("sensors", agent);
		for (auto i = sensorsJson.begin(); i != sensorsJson.end(); i++)
		{
			string sensorName = i->get<string>();
			bool found = false;
			// Look for registered sensor
			
			if (!found)
				for (std::set<std::string>::iterator itr = extSensors.begin(); itr != extSensors.end(); itr++)
				{
					if (sensorName == *itr)
					{
						a.extSensors.push_back(shared_ptr<ExternalSensor>(InstantiateExternalSensor(*itr)));
						found = true;
						break;
					}
				}
			
			if (!found)
				for (std::set<std::string>::iterator itr = intSensors.begin(); itr != intSensors.end(); itr++)
				{
					if (sensorName == *itr)
					{
						a.intSensors.push_back(shared_ptr<InternalSensor>(InstantiateInternalSensor(*itr)));
						found = true;
						break;
					}
				}	
			
			if (!found)
				Error("SimulatorConfiguration::ReadAgent", string("Sensor ") + sensorName + " in agent " + a.GetID() + " not found.");
		}
	}
	catch (out_of_range&)
	{}
	
	
	// ========== Add controller, it's mandatory ============
	// TODO Could be optional. 0 control by default
	try
	{
		json controllerJson = GetEntry("control_model", agent);
		json controlFailuresJson;
		std::vector<std::string> controlFailureNames;
		
		try
		{
			controlFailuresJson = GetEntry("control_failures", agent);
			if (!controlFailuresJson.is_array())
				Error("SimulatorConfiguration::ReadAgent", "\'control_failures\' entry must be an array");
			
			for (auto fail = controlFailuresJson.begin(); fail != controlFailuresJson.end(); fail++)
				controlFailureNames.push_back(fail->get<string>());
			
		}
		catch(out_of_range&)
		{
		}
		
		// TODO add a check like this to the other entries
		if (!controllerJson.is_string())
			Error("SimulatorConfiguration::ReadAgent", "\'control_model\' entry should be a string");
		
		string modelName = controllerJson.get<string>();
		// Now look for this name in the list of declared control_models
		for (auto model = controlModels.begin(); model != controlModels.end();
			 model++)
			 {
				if (modelName == model->GetName())
				{
					if (a.controller || a.automaton)
						Error("SimulatorConfiguration::ReadAgent", "BUG FIXME! Why is agent's controller or automaton already filled?");
					
					a.controller = InstantiateController(model->GetControllerName());
					a.controller->SetControlModel(*model);
					
					std::vector<ControllerFailure> failures;
					
					for (auto fail = controlFailureNames.begin(); fail != controlFailureNames.end(); fail++)
					{
						ControllerFailure f;
						
						f.SetFailureFunction(InstantiateControllerFailure(*fail));
						f.SetName(*fail);
						failures.push_back(f);
					}
					
					a.controller->SetFailures(failures);
					
					a.automaton = InstantiateAutomaton(model->GetAutomatonName());
					a.automaton->SetPossibleManeuvers(model->GetManeuvers());
					a.automaton->DefineRules();
					break;
				}
			 }
	}
	catch(out_of_range&)
	{
		Error("SimulatorConfiguration::ReadAgent", "\'control_model\' entry is mandatory" /* see TODO*/);
	}
	
	
	try
	{
		json initManJson = GetEntry("init_maneuver", agent);
		if (!initManJson.is_string())
			Error("SimulatorConfiguration::ReadAgent", "\'init_maneuver\' entry must be a string");
		
		string initMan = initManJson.get<string>();
		a.automaton->SetManeuver(initMan);
	}
	catch(out_of_range&)
	{
		Error("SimulatorConfiguration::ReadAgent", "\'init_maneuver\' entry is mandatory");
	}
	
	return a;
    
}

// TODO check if two equal named sensors are declared
void SimulatorConfiguration::ReadSensor(const nlohmann::json& sensorJson)
{
	// Look for unknown entries
	for (auto itr = sensorJson.begin(); itr != sensorJson.end(); itr++)
	{
		if (itr.key() != "name" &&
			itr.key() != "type")
			Error("SimulatorConfiguration::ReadSensor", string("Unknown sensor entry ") + itr.key());
	}
	
	try
	{
		string name = GetEntry("name", sensorJson).get<string>();
		string type = GetEntry("type", sensorJson).get<string>();
		
		
		if (type == "internal" || type == "Internal" || type == "INTERNAL")
			intSensors.insert(name);
		else if (type == "external" || type == "External" || type == "EXTERNAL")
			extSensors.insert(name);
		else
			Error("SimulatorConfiguration::ReadSensor", string("Unknown sensor type: ") + type + " --- please define sensors as either \'internal\' or \'external\'");
		
	}
	catch(out_of_range&)
	{
			Error("SimulatorConfiguration::ReadSensor", "name, type and measured_world_variables are mandatory sensor entries");
	}
}

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

const SimulationParameters& SimulatorConfiguration::GetCustomEntries() const
{
	return simCustomEntries;
}

const EnvironmentParameters& SimulatorConfiguration::GetEnvironmentParameters() const
{
	return envParameters;
}

const double& SimulatorConfiguration::GetEnvironmentParameter(const std::string& parName) const
{
	return envParameters(parName);
}

const AgentCustomEntries & SimulatorConfiguration::GetAgentCustomEntries(const std::string& ID) const
{	
	const AgentCustomEntries* entry = nullptr;
	
	try
	{
		entry = &agentsCustomEntries.at(ID);
	}
	catch (out_of_range&)
	{
		Error("SimulatorConfiguration::GetAgentParameters", string("Agent ") + ID + " not found");
	}
	
	return *entry;
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
			if (!stateVarsJson.is_array())
				Error("SimulatorConfiguration::AddDynamicModel", "state_variables entry in dynamic_models must contain an array [] of state variable names");
			
			vector<string> stateVars;
			for (auto stateIndex = stateVarsJson.begin(); 
				 stateIndex != stateVarsJson.end(); stateIndex++)
					stateVars.push_back(stateIndex->get<string>());
			
			m.SetStateVariables(stateVars);
			isStateVarSet = true;
		}
		else if (itr.key() == "dynamics")
		{
			m.SetDynamicsFunctionName(itr.value().get<string>());
			m.SetDynamicsFunction(GetDynamicsFunction(itr.value().get<string>()));
			isDynamicsFunctionSet = true;
		}
		else if (itr.key() == "control_variables")
		{
			json controlVarsJson = itr.value();
			if (!controlVarsJson.is_array())
				Error("SimulatorConfiguration::AddDynamicModel", "control_variables entry in dynamic_models must contain an array [] of control variable names");
			
			vector<string> controlVars;
			for (auto controlIndex = controlVarsJson.begin(); 
				 controlIndex != controlVarsJson.end(); controlIndex++)
					controlVars.push_back(controlIndex->get<string>());
			
			m.SetControlVariables(controlVars);
			isControlVarSet = true;
		}
		else if (itr.key() == "world_conversion_function")
		{
			m.SetStateConversionToWorldName(itr.value().get<string>());
			m.SetStateConversionToWorld(GetStateConversionToWorld(itr.value().get<string>()));
			isStateConversionFunctionSet = true;
		}
		else if (itr.key() == "state_estimation_function")
		{
			m.SetStateConversionToStateName(itr.value().get<string>());
			m.SetStateConversionToState(GetStateConversionToState(itr.value().get<string>()));
		}
		else
			Error("SimulatorConfiguration::AddDynamicModel", string("Unknown entry \'") + itr.key() + "\'. Dynamic models must each contain a name, a set of state variables, a dynamics function name, and a set of control variables");
	}
	
	if (!isNameSet || !isStateVarSet || !isDynamicsFunctionSet || !isControlVarSet || !isStateConversionFunctionSet)
		Error("SimulatorConfiguration::AddDynamicModel", "Missing entry. Dynamic models must each contain a name, a set of state variables, a dynamics function name, a set of control variables, and a conversion function from states to world coordinates");
	
	dynamicModels.insert(m);
	
}

void SimulatorConfiguration::AddControlModel ( const nlohmann::json & modelJ)
{
	ControlModel m;
	
	bool isNameSet = false, isManeuversSet = false, isControlVarSet = false, isControllerSet = false, isAutomatonSet = false;
	
	
	for (auto itr = modelJ.begin(); itr != modelJ.end(); itr++)
	{
		if (itr.key() == "name")
		{
			m.SetName(itr.value().get<string>());
			isNameSet = true;
		}
		else if (itr.key() == "maneuvers")
		{
			json maneuversJson = itr.value();
			if (!maneuversJson.is_array())
				Error("SimulatorConfiguration::AddControlModel", "\'maneuvers\' entry in control_models must contain an array [] of state variable names");
			
			vector<Maneuver> maneuvers;
			for (auto index = maneuversJson.begin(); 
				 index != maneuversJson.end(); index++)
					maneuvers.push_back(Maneuver(index->get<string>()));
			
			m.SetManeuvers(maneuvers);
			isManeuversSet = true;
		}
		else if (itr.key() == "controller")
		{
			m.SetController(itr.value().get<string>());
			isControllerSet = true;
		}
		else if (itr.key() == "control_variables")
		{
			json controlVarsJson = itr.value();
			if (!controlVarsJson.is_array())
				Error("SimulatorConfiguration::AddDynamicModel", "control_variables entry in dynamic_models must contain an array [] of control variable names");
			
			vector<string> controlVars;
			for (auto controlIndex = controlVarsJson.begin(); 
				 controlIndex != controlVarsJson.end(); controlIndex++)
					controlVars.push_back(controlIndex->get<string>());
			
			m.SetControlVariables(controlVars);
			isControlVarSet = true;
		}
		else if (itr.key() == "automaton")
		{
			m.SetAutomaton(itr.value().get<string>());
			isAutomatonSet = true;
		}
		else
			Error("SimulatorConfiguration::AddControlModel", "Unknown entry \'" + itr.key() + "\'. Control models must each contain a name, a set of maneuvers, a controller class name, a set of controller variables, and an automaton");
	}
	
	if (!isNameSet || !isControlVarSet || !isManeuversSet || !isControllerSet || !isAutomatonSet)
		Error("SimulatorConfiguration::AddControllerModel", "Missing entry. Control models must each contain a name, a set of maneuvers, a controller class name, a set of controller variables, and an automaton");
	
	controlModels.insert(m);
	
}


const WorldAgentFeatures & SimulatorConfiguration::GetWorldAgentFeatures() const
{
	return agentFeatures;
}

const WorldEnvironmentFeatures & SimulatorConfiguration::GetWorldEnvironmentFeatures() const
{
	return envFeatures;
}


