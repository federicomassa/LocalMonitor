#include "SimulAgent.h"
#include "SimulatorConfiguration.h"
#include "Utility/LogFunctions.h"
#include "Utility/MyLogger.h"
#include "Automation/DynamicModel.h"

// Automatically created header during configure
// It contains all the dynamic models declared in the config file
#include "Input/Dynamics/DynamicModels.h"
#include "Input/Automation/Controllers/Controllers.h"
#include "Input/Automation/Automatons/Automatons.h"
#include "Input/Observers/Observers.h"

#include "Environment.h"

#include <iostream>

using namespace std;
using namespace LogFunctions;

SimulAgent::SimulAgent(SimulatorConfiguration* config) : controller(nullptr), automaton(nullptr)
{	
	conf = config;
	
	if (config)
		pLayer.SetSimulationTimeStep(config->GetSimulationTimeStep());
	
	environment = nullptr;
}

SimulAgent::~SimulAgent()
{
	if (controller)
		delete controller;
	
	if (automaton)
		delete automaton;
}

// FIXME Automaton does not use a copy method. It's not modular and does not copy everything. For now it's okay because the copy happens at config time
SimulAgent::SimulAgent(const SimulAgent& a) : pLayer(a.pLayer)
{	
	agent = a.agent;
	
	// SimulAgent has ownership --> Pointers are reinstantiated, not copied
	controller = InstantiateController(a.controller->GetName());
	controller->SetControlModel(a.controller->GetControlModel());
	controller->SetFailures(a.controller->GetFailures());
	
	automaton = InstantiateAutomaton(a.automaton->GetName());
	automaton->DefineRules();
	automaton->SetManeuver(a.automaton->GetManeuver());
	automaton->SetPossibleManeuvers(a.automaton->GetPossibleManeuvers());
	
	conf = a.conf;
	
	worldState = a.worldState;
	extSensors = a.extSensors;
	intSensors = a.intSensors;
	
	environment = a.environment;
}

SimulAgent& SimulAgent::operator=(const SimulAgent& a)
{		
	if (controller)
		delete controller;
	
	if (automaton)
		delete automaton;
	
	agent = a.agent;
	controller = InstantiateController(a.controller->GetName());
	controller->SetControlModel(a.controller->GetControlModel());
	controller->SetFailures(a.controller->GetFailures());

	automaton = InstantiateAutomaton(a.automaton->GetName());
	automaton->DefineRules();
	automaton->SetManeuver(a.automaton->GetManeuver());
	automaton->SetPossibleManeuvers(a.automaton->GetPossibleManeuvers());

	
	conf = a.conf;
	pLayer = a.pLayer;
	
	worldState = a.worldState;
	
	for (auto itr = a.extSensors.begin(); itr != a.extSensors.end(); itr++)
		extSensors.push_back(*itr);

	for (auto itr = a.intSensors.begin(); itr != a.intSensors.end(); itr++)
		intSensors.push_back(*itr);
	
	environment = a.environment;
	
	return *this;
}

void SimulAgent::SetEnvironment(const Environment* e)
{
	environment = e;
}

const string &SimulAgent::GetID() const
{
    return agent.GetID();
}

void SimulAgent::SetID(const string& str)
{
    agent.SetID(str);
}
const State &SimulAgent::GetState() const
{
    return agent.GetState();
}

const DynamicModel& SimulAgent::GetDynamicModel() const
{
	return pLayer.GetDynamicModel();
}

const Automaton * SimulAgent::GetAutomaton() const
{
	return automaton;
}

const Controller * SimulAgent::GetController() const
{
	return controller;
}

void SimulAgent::SetState(const State& q)
{
    agent.SetState(q);
}

const Maneuver& SimulAgent::GetManeuver() const
{
	return automaton->GetManeuver();
}

//TODO Check compatibility between model and function
void SimulAgent::SetDynamicModel(const DynamicModel& m)
{
	pLayer.SetDynamicModel(m);
}


State SimulAgent::GenerateWorldState(const State& worldState)
{
	this->worldState = pLayer.GetDynamicModel().GetWorldState(agent, worldState);
	return (this->worldState);
}

const State& SimulAgent::GetWorldState() const
{
	return worldState;
}


const Agent& SimulAgent::GetAgent() const
{
	return agent;
}

MyLogger &operator<<(MyLogger &os, const SimulAgent &a)
{
    os << a.agent;
    return os;
}


void SimulAgent::Run(const SensorOutput& sensorOutput, const double& currentTime)
{
	SendToController(sensorOutput, currentTime);
	SendToAutomaton(sensorOutput, currentTime);
	vector<string> controlVars = pLayer.GetDynamicModel().GetControlVariables();
	Control control = Control::GenerateStateOfType(controlVars);
	controller->Run(control, GetManeuver(), currentTime);
	
	// So to be able to write it to viewer if necessary
	controller->SaveControl(control);
	
	// Get true others from environment
	Require(environment != nullptr, "SimulAgent::Run", "Environment is not set. Please call SimulAgent::SetEnvironment first");
	
	const SimulAgentVector& trueSimulOthersInWorld = environment->GetAgents();
	
	// vector containing all other agents expressed in world coordinates
	AgentVector trueOthersInWorld;
	
	for (auto itr = trueSimulOthersInWorld.begin(); itr != 
		trueSimulOthersInWorld.end(); itr++)
	{
		if (itr->first == GetID())
			continue;
		
		trueOthersInWorld[itr->first].SetState(itr->second.GetWorldState());
	}
	
    agent.SetState(pLayer.GetNextState(agent, trueOthersInWorld, control));
	
	automaton->PreEvolve();
	automaton->Evolve();
	automaton->PostEvolve();	
	
	SendToLocalMonitor(sensorOutput, currentTime);
	localMonitor.Run(currentTime);
}

void SimulAgent::SendToController(const SensorOutput& sensorOutput, const double& currentTime)
{
	controller->ReceiveSensorOutput(sensorOutput, currentTime);
}

void SimulAgent::SendToAutomaton(const SensorOutput& sensorOutput, const double& currentTime)
{
	automaton->ReceiveSensorOutput(sensorOutput, currentTime);
}

void SimulAgent::SendToLocalMonitor(const SensorOutput& sensorOutput, const double& currentTime)
{
	localMonitor.ReceiveSensorOutput(sensorOutput, currentTime);
}



void SimulAgent::SetParameters(const AgentParameters& pars)
{
	agent.SetParameters(pars);
}

const AgentParameters & SimulAgent::GetParameters() const
{
	return agent.GetParameters();
}


ExternalSensorOutput SimulAgent::RetrieveExternalSensorOutput(const std::string& sensorName, const Agent& selfInWorld, const AgentVector& othersInWorld, const EnvironmentParameters& envParams)
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
	
	sensor->SimulateVisibility(visibleRegion, visibleIDs, selfInWorld, othersInWorld);
	
	// Build vector of visible agents (with true states) and compute sensor output for those agents
	AgentVector trueVisibleAgents;
	for (auto visibleID = visibleIDs.begin(); visibleID != visibleIDs.end(); visibleID++)
	{
		trueVisibleAgents[*visibleID] = othersInWorld.at(*visibleID);
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
		newAgent.SetParameters(visibleAgent->second.GetParameters());
		
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
	sensor->SimulateOutput(output, selfInWorld, othersInWorld, envParams);
		
	return output;
	
}

InternalSensorOutput SimulAgent::RetrieveInternalSensorOutput(const std::string& sensorName, const Agent& trueSelfInWorld)
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
	
	// FIXME Should only set parameters measured by sensor
	measuredSelf.SetParameters(trueSelfInWorld.GetParameters());
	
	// Compute the output
	InternalSensorOutput output;
	output.SetMeasuredSelf(measuredSelf);
	sensor->SimulateOutput(output, trueSelfInWorld);
	
	return output;
}

SensorOutput SimulAgent::SimulateSensors(const Agent& trueSelfInWorld, const AgentVector& trueOthersInWorld, const EnvironmentParameters& trueEnvParams)
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

void SimulAgent::InitializeLocalMonitor(const std::string& configFilePath)
{
	localMonitor.Configure(GetID(), configFilePath);
}

