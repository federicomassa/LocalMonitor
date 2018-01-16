#include "NaiveObserver.h"
#include "Utility/LogFunctions.h"
#include "Automation/DynamicModel.h"
#include "Automation/ControlModel.h"
#include "Dynamics/DynamicModels.h"
#include "Utility/MyLogger.h"
#include "Dynamics/StateConversions.h"
#include "Automation/Automatons/Automatons.h"
#include "Automation/Controllers/Controllers.h"
#include "Automation/Sensors/Sensors.h"
#include <iostream>
#include <vector>
#include <cmath>

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
	mandatoryEntries.push_back("id");
	mandatoryEntries.push_back("type");
	mandatoryEntries.push_back("dynamic_model");
	mandatoryEntries.push_back("control_model");
	mandatoryEntries.push_back("simulation_time_step");
	mandatoryEntries.push_back("prediction_time_span");
	mandatoryEntries.push_back("range");
	mandatoryEntries.push_back("resolution");

	if (!observingJson.at("id").is_string())
		Error("NaiveObserver::Configure", "\'id\' entry should be a string");
	
	observedID = observingJson.at("id").get<string>();
	
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
	if (!dynJson.is_array())
		Error("NaiveObserver::Configure"; "\'dynamic_model\' entry must be a JSON array");
	
	for (auto itr = dynJson.begin(); itr != dynJson.end(); itr++)
		ReadDynamicModel(*itr);
	
	nlohmann::json controlJson = observingJson.at("control_model");
	ReadControlModel(controlJson);
	
	pLayer.SetSimulationTimeStep(observingJson.at("simulation_time_step").get<double>());
	
	nlohmann::json rangeJson = observingJson.at("range");
	if (!rangeJson.is_object())
		Error("NaiveObserver::Configure", "\'range\' entry must be a json object");
	
	ReadRange(rangeJson);
	
	
	nlohmann::json resJson = observingJson.at("resolution");
	if (!rangeJson.is_object())
		Error("NaiveObserver::Configure", "\'resolution\' entry must be a json object");
	
	ReadResolution(resJson);
	
	// EXTERNAL SENSORS
	try
	{
		nlohmann::json extSensJson = observingJson.at("external_sensors");
		if (!extSensJson.is_array())
			Error("NaiveObserver::Configure", "\'external_sensors\' entry must be a json array of strings");
	
		if (extSensJson.size() == 0)
			throw out_of_range("\'external_sensors\'");
		
		for (auto itr = extSensJson.begin(); itr != extSensJson.end(); itr++)
			extSensors.push_back(shared_ptr<ExternalSensor>(InstantiateExternalSensor(itr->get<string>())));
	}
	catch(out_of_range&)
	{
		Info("NaiveObserver::Configure", string("No external sensors defined in \'observing\' agent with ID \'") + observingJson.at("id").get<string>() + "\'."); 
	}
	

	// INTERNAL SENSORS
	try
	{
		nlohmann::json intSensJson = observingJson.at("internal_sensors");
		if (!intSensJson.is_array())
			Error("NaiveObserver::Configure", "\'external_sensors\' entry must be a json array of strings");
	
		if (intSensJson.size() == 0)
			throw out_of_range("\'internal_sensors\'");
		
		for (auto itr = intSensJson.begin(); itr != intSensJson.end(); itr++)
			intSensors.push_back(shared_ptr<InternalSensor>(InstantiateInternalSensor(itr->get<string>())));
	}
	catch(out_of_range&)
	{
		Info("NaiveObserver::Configure", string("No internal sensors defined in \'observing\' agent with ID \'") + observingJson.at("id").get<string>() + "\'."); 
	}

	
	// FIXME Someday this will be computed for each run because it might depend on visibility
	for (auto itr = pLayer.GetDynamicModel().GetStateVariables().begin(); 
		 itr != pLayer.GetDynamicModel().GetStateVariables().end(); itr++)
		 nPredictions[*itr] = ceil((varRange[*itr].second - varRange[*itr].first)/varResolution[*itr]);

	auto itr = nPredictions.begin();
	InitializeHiddenState(itr);
}

void NaiveObserver::ReadDynamicModel(const nlohmann::json& dynJ)
{	
	// ========= CHECK MANDATORY ENTRIES ============	
	vector<string> mandatoryEntries;
	mandatoryEntries.push_back("id");
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
	
	
	pLayer[dynJ.at("id").get<string>()].SetDynamicModel(dynModel);
}

void NaiveObserver::ReadControlModel(const nlohmann::json& controlJ)
{
	// ========= CHECK MANDATORY ENTRIES ============
	vector<string> mandatoryEntries;
	mandatoryEntries.push_back("id");
	mandatoryEntries.push_back("name");
	mandatoryEntries.push_back("maneuvers");
	mandatoryEntries.push_back("controller");
	mandatoryEntries.push_back("control_variables");
	mandatoryEntries.push_back("automaton");
	
	// This is mandatory for each non observed agent
	if (controlJ.at("id").get<string>() != observedID)
		mandatoryEntries.push_back("init_maneuver");
	
	
	for (auto itr = mandatoryEntries.begin();  itr != mandatoryEntries.end(); itr++)
	{
		try
		{
			controlJ.at(*itr);
		}
		catch(out_of_range&)
		{
			Error("NaiveObserver::ReadControlModel", string("Mandatory entry \'") + *itr + "\' not found in observer configuration file, inside observing entry of agent " + GetObservedID() + ", in control_model"); 
		}
	}
	// =============================================
	
	controller[controlJ.at("id").get<string>()] = InstantiateController(controlJ.at("controller").get<string>());
	
	// FIXME Set but not simulated
	automaton[controlJ.at("id").get<string>()] = InstantiateAutomaton(controlJ.at("automaton").get<string>());
	
	vector<string> controlVars;
	json controlVarsJ = controlJ.at("control_variables");
	
	if (!controlVarsJ.is_array())
		Error("NaiveObserver::ReadDynamicModel", "control_variables should be an array");
	
	for (auto itr = controlVarsJ.begin(); itr != controlVarsJ.end(); itr++)
		controlVars.push_back(itr->get<string>());
	
	Require(pLayer.at(controlJ.at("id").get<string>()).GetDynamicModel().GetControlVariables() == controlVars, "NaiveObserver::ReadControlModel", "control_model and dynamic_model must have same control variables");
	
	// When defining models of other agents (not the observed one, you 
	// must specified an init_maneuver that remains the same (the automaton is not actually run). This is a simplified model 
	if (controlJ.at("id").get<string>() != observedID)
	{
		
	}	
}

void NaiveObserver::ReadRange(const nlohmann::json& rangeJ)
{
	// ========= CHECK MANDATORY ENTRIES ============
	
	// There must be a range specified for each state variable of the model
	vector<string> mandatoryEntries;
	for (auto itr = pLayer.GetDynamicModel().GetStateVariables().begin();
		 itr != pLayer.GetDynamicModel().GetStateVariables().end(); itr++)
		 mandatoryEntries.push_back(*itr);
	
	for (auto itr = mandatoryEntries.begin();  itr != mandatoryEntries.end(); itr++)
	{
		try
		{
			nlohmann::json varJ = rangeJ.at(*itr);
			if (!varJ.is_array() || varJ.size() != 2)
				Error("NaiveObserver::ReadRange", "In \'range\' entry, for each state variable there must be an array of two elements, min and max value");
			
			if (varJ.at(0).get<double>() > varJ.at(1).get<double>())
				Error("NaiveObserver::ReadRange", "In \'range\' entry there must be a min and max value, the first being smaller than the second.");
			
			varRange[*itr] = std::make_pair(varJ.at(0).get<double>(), varJ.at(1).get<double>());
			
		}
		catch(out_of_range&)
		{
			Error("NaiveObserver::ReadControlModel", string("Mandatory entry \'") + *itr + "\' not found in observer configuration file, inside observing entry of agent " + GetObservedID() + ", in \'range\' entry"); 
		}
	}
	// =============================================
}


void NaiveObserver::ReadResolution(const nlohmann::json& resJ)
{
	// ========= CHECK MANDATORY ENTRIES ============
	
	// There must be a range specified for each state variable of the model
	vector<string> mandatoryEntries;
	for (auto itr = pLayer.GetDynamicModel().GetStateVariables().begin();
		 itr != pLayer.GetDynamicModel().GetStateVariables().end(); itr++)
		 mandatoryEntries.push_back(*itr);
	
	for (auto itr = mandatoryEntries.begin();  itr != mandatoryEntries.end(); itr++)
	{
		try
		{
			nlohmann::json varJ = resJ.at(*itr);
			if (!varJ.is_number())
				Error("NaiveObserver::ReadRange", "In \'resolution\' entry, for each state variable there must be a float that indicates the resolution of the hidden agent mapping");
			
			varResolution[*itr] = varJ.get<double>();
			
		}
		catch(out_of_range&)
		{
			Error("NaiveObserver::ReadControlModel", string("Mandatory entry \'") + *itr + "\' not found in observer configuration file, inside observing entry of agent " + GetObservedID() + ", in \'resolution\' entry"); 
		}
	}
	// =============================================
}

void NaiveObserver::ReceiveSensorOutput(const SensorOutput& sensorOutput, const double& currentTime)
{
	// !!! Sensor data has to be reorganized so that 'self' is observed agent and 'others' are its neighbors. NB Trajectories are stored in world coordinates, but for the prediction they have to be converted to state variables, which is the state estimation phase !!!
		
	const Agent& currentSelf = sensorOutput.RetrieveSelfData();
	const AgentVector& currentOthers = sensorOutput.RetrieveOtherAgentsData();
	const EnvironmentParameters& currentEnv = sensorOutput.RetrieveEnvironmentData();
	
	
	selfTrajectory.clear();
	othersTrajectory.clear();
	environmentTrajectory.clear();
	
	// These are the 'truth' values of the trajectories that the observed agent sees. They will be filtered by observed agent's sensors.
	Agent trueSelf;
	AgentVector trueOthers;
	const EnvironmentParameters& trueEnv = currentEnv;
	
	// Look for observed agent in 'others', that will be the true self
	for (auto itr = currentOthers.begin(); itr != currentOthers.end(); itr++)
	{
		if (itr->second.GetID() == observedID)
			trueSelf = itr->second;
		else
			trueOthers[itr->first] = itr->second;
	}
	
	// Now that we have the truth values, we have to filter them with the sensors
	SensorOutput observedSensorOutput = SimulateSensors(trueSelf, trueOthers, trueEnv);
	
	
	selfTrajectory.insert(currentTime, observedSensorOutput.RetrieveSelfData());
	
	othersTrajectory.insert(currentTime, observedSensorOutput.RetrieveOtherAgentsData());
	
	environmentTrajectory.insert(currentTime, observedSensorOutput.RetrieveEnvironmentData());
}

SensorOutput NaiveObserver::SimulateSensors(const Agent& trueSelfInWorld, const AgentVector& trueOthersInWorld, const EnvironmentParameters& trueEnvParams)
{
	SensorOutput sensorOutput;
	
	// Call external external sensors
	for (auto extSensor = extSensors.begin(); extSensor != extSensors.end(); extSensor++)
	{
		ExternalSensorOutput extOutput = RetrieveExternalSensorOutput((*extSensor)->GetName(), trueSelfInWorld, trueOthersInWorld, trueEnvParams);
		
		sensorOutput.MergeExternalSensor(extOutput);
	}

	// Call internal sensors
	for (auto intSensor = intSensors.begin(); intSensor != intSensors.end(); intSensor++)
	{
		InternalSensorOutput intOutput = RetrieveInternalSensorOutput((*intSensor)->GetName(), trueSelfInWorld);
		
		sensorOutput.MergeInternalSensor(intOutput);
	}

	return sensorOutput;
	
}

ExternalSensorOutput NaiveObserver::RetrieveExternalSensorOutput(const std::string& sensorName, const Agent& trueSelfInWorld, const AgentVector& trueOthersInWorld, const EnvironmentParameters& trueEnvParams)
{
	ExternalSensor* sensor = nullptr;
	for (auto itr = extSensors.begin(); itr != extSensors.end(); itr++)
	{
		if ((*itr)->GetName() == sensorName)
			sensor = itr->get();
	}
	
	if (sensor == nullptr)
		Error("SimulAgent::RetrieveExternalSensorData", string("Sensor \'") + sensorName + "\'"  + "not found in SimulAgent sensor list");
	
	StateRegion visibleRegion;
	std::set<std::string> visibleIDs;
	
	sensor->SimulateVisibility(visibleRegion, visibleIDs, trueSelfInWorld, trueOthersInWorld);
	
	// Build vector of visible agents (with true states) and compute sensor output for those agents
	AgentVector trueVisibleAgents;
	for (auto visibleID = visibleIDs.begin(); visibleID != visibleIDs.end(); visibleID++)
	{
		trueVisibleAgents[*visibleID] = trueOthersInWorld.at(*visibleID);
	}
	
	// ================ Prepare variables for to call sensor output =================
	ExternalSensor::SensorVars agentVars = sensor->GetMeasuredAgentVariables();
	
	State measuredState = State::GenerateStateOfType(agentVars);
	
	// measuredVisibleAgents will contain the output of the sensor
	AgentVector measuredVisibleAgents;
	for (auto visibleAgent = trueVisibleAgents.begin(); visibleAgent != trueVisibleAgents.end(); visibleAgent++)
	{
		// Create agent with true ID and empty state with vars that can be measured by this sensor
		Agent newAgent;
		newAgent.SetID(visibleAgent->first);
		newAgent.SetState(measuredState);
		 
		// FIXME This should set only parameters measured by sensor.
		//newAgent.SetParameters(visibleAgent->second.GetParameters());
		
		measuredVisibleAgents[newAgent.GetID()] = newAgent;
	}
	
	// Now we do something similar for the environment measurements
	ExternalSensor::SensorVars envVars = sensor->GetMeasuredEnvironmentVariables();
	EnvironmentParameters measuredEnvParameters;
	
	for (auto itr = envVars.begin(); itr != envVars.end(); itr++)
		measuredEnvParameters.AddEntry(*itr, 0.0);
	
	// Compute the output
	ExternalSensorOutput output;
	output.SetMeasuredAgents(measuredVisibleAgents);
	output.SetMeasuredEnvironment(measuredEnvParameters);
	
	// TODO set visible region?
	
	// By default, set 
	sensor->SimulateOutput(output, trueSelfInWorld, trueOthersInWorld, trueEnvParams);
		
	return output;
}

InternalSensorOutput NaiveObserver::RetrieveInternalSensorOutput(const std::string& sensorName, const Agent& trueSelfInWorld)
{
	const InternalSensor* sensor = nullptr;
	for (auto itr = intSensors.begin(); itr != intSensors.end(); itr++)
	{
		if ((*itr)->GetName() == sensorName)
			sensor = itr->get();
	}
	
	if (sensor == nullptr)
		Error("SimulAgent::RetrieveInternalSensorData", string("Sensor \'") + sensorName + "\'"  + "not found in SimulAgent sensor list");
	
	
	// ================ Prepare variables for to call sensor output =================
	InternalSensor::SensorVars agentVars = sensor->GetSelfMeasuredVariables();
	State measuredSelfState = State::GenerateStateOfType(agentVars);
	
	Agent measuredSelf;
	measuredSelf.SetID(trueSelfInWorld.GetID());
	measuredSelf.SetState(measuredSelfState);
	//measuredSelf.SetParameters(trueSelfInWorld.GetParameters());
	
	// Compute the output
	InternalSensorOutput output;
	output.SetMeasuredSelf(measuredSelf);
	sensor->SimulateOutput(output, trueSelfInWorld);
	
	return output;
}

void NaiveObserver::InitializeHiddenState()
{
	
}


