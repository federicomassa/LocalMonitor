#include "NaiveObserver.h"
#include "Utility/LogFunctions.h"
#include "Automation/DynamicModel.h"
#include "Automation/ControlModel.h"
#include "Dynamics/DynamicModels.h"
#include "Dynamics/StateConversions.h"
#include "Automation/Automatons/Automatons.h"
#include "Automation/Controllers/Controllers.h"
#include "Automation/Sensors/Sensors.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace LogFunctions;
using namespace nlohmann;

// TODO: there is no check that automaton's transitions are among maneuvers in control model 

NaiveObserver::NaiveObserver(const std::string& name) : Observer(name)
{
}

NaiveObserver::~NaiveObserver()
{
	if (automaton)
		delete automaton;
	
	if (controller)
		delete controller;
}


void NaiveObserver::Run()
{
}

void NaiveObserver::Configure(const nlohmann::json& observingJson)
{
	// ========= CHECK MANDATORY ENTRIES ============
	
	vector<string> mandatoryEntries;
	mandatoryEntries.push_back("dynamic_model");
	mandatoryEntries.push_back("control_model");
	mandatoryEntries.push_back("simulation_time_step");

	
	for (auto itr = mandatoryEntries.begin();  itr != mandatoryEntries.end(); itr++)
	{
		try
		{
			observingJson.at(*itr);
		}
		catch(out_of_range&)
		{
			Error("NaiveObserver::Configure", string("Mandatory entry ") + *itr + " not found in observer configuration file, inside observing entry of agent " + GetObservedID()); 
		}
	}
	// =============================================
	
	nlohmann::json dynJson = observingJson.at("dynamic_model");
	ReadDynamicModel(dynJson);
	
	nlohmann::json controlJson = observingJson.at("control_model");
	ReadControlModel(controlJson);
	
	pLayer.SetSimulationTimeStep(observingJson.at("simulation_time_step").get<double>());
}

void NaiveObserver::ReadDynamicModel(const nlohmann::json& dynJ)
{	
	// ========= CHECK MANDATORY ENTRIES ============	
	vector<string> mandatoryEntries;
	mandatoryEntries.push_back("name");
	mandatoryEntries.push_back("state_variables");
	mandatoryEntries.push_back("dynamics");
	mandatoryEntries.push_back("control_variables");
	mandatoryEntries.push_back("world_conversion_function");
	
	for (auto itr = mandatoryEntries.begin();  itr != mandatoryEntries.end(); itr++)
	{
		try
		{
			dynJ.at(*itr);
		}
		catch(out_of_range&)
		{
			Error("NaiveObserver::ReadDynamicModel", string("Mandatory entry ") + *itr + " not found in observer configuration file, inside observing entry of agent " + GetObservedID() + ", in dynamic_model"); 
		}
	}
	// =============================================

	DynamicModel dynModel;
	
	dynModel.SetName(dynJ.at("name").get<string>());
	
	vector<string> stateVars;
	json stateVarsJ = dynJ.at("state_variables");
	
	if (!stateVarsJ.is_array())
		Error("NaiveObserver::ReadDynamicModel", "state_variables should be an array");
	
	for (auto itr = stateVarsJ.begin(); itr != stateVarsJ.end(); itr++)
		stateVars.push_back(itr->get<string>());
		
	dynModel.SetStateVariables(stateVars);
	
	dynModel.SetDynamicsFunctionName(dynJ.at("dynamics").get<string>());	dynModel.SetDynamicsFunction(GetDynamicsFunction(dynJ.at("dynamics").get<string>()));
	
	vector<string> controlVars;
	json controlVarsJ = dynJ.at("control_variables");
	
	if (!controlVarsJ.is_array())
		Error("NaiveObserver::ReadDynamicModel", "control_variables should be an array");
	
	for (auto itr = controlVarsJ.begin(); itr != controlVarsJ.end(); itr++)
		controlVars.push_back(itr->get<string>());
	
	dynModel.SetControlVariables(controlVars);
	dynModel.SetStateConversionFunctionName(dynJ.at("world_conversion_function").get<string>());	dynModel.SetStateConversionFunction(GetStateConversionFunction(dynJ.at("world_conversion_function").get<string>()));
	
	
	pLayer.SetDynamicModel(dynModel);
}

void NaiveObserver::ReadControlModel(const nlohmann::json& controlJ)
{
	// ========= CHECK MANDATORY ENTRIES ============
	vector<string> mandatoryEntries;
	mandatoryEntries.push_back("name");
	mandatoryEntries.push_back("maneuvers");
	mandatoryEntries.push_back("controller");
	mandatoryEntries.push_back("control_variables");
	mandatoryEntries.push_back("automaton");
	
	for (auto itr = mandatoryEntries.begin();  itr != mandatoryEntries.end(); itr++)
	{
		try
		{
			controlJ.at(*itr);
		}
		catch(out_of_range&)
		{
			Error("NaiveObserver::ReadControlModel", string("Mandatory entry ") + *itr + " not found in observer configuration file, inside observing entry of agent " + GetObservedID() + ", in control_model"); 
		}
	}
	// =============================================
	
	controller = InstantiateController(controlJ.at("controller").get<string>());
	automaton = InstantiateAutomaton(controlJ.at("automaton").get<string>());
	
	vector<string> controlVars;
	json controlVarsJ = controlJ.at("control_variables");
	
	if (!controlVarsJ.is_array())
		Error("NaiveObserver::ReadDynamicModel", "control_variables should be an array");
	
	for (auto itr = controlVarsJ.begin(); itr != controlVarsJ.end(); itr++)
		controlVars.push_back(itr->get<string>());
	
	Require(pLayer.GetDynamicModel().GetControlVariables() == controlVars, "NaiveObserver::ReadControlModel", "control_model and dynamic_model must have same control variables");
	
	
}

