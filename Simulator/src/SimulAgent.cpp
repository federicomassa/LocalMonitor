#include "SimulAgent.h"
#include "SimulatorConfiguration.h"
#include "Utility/LogFunctions.h"
#include "Utility/Logger.h"
#include "Automation/DynamicModel.h"

// Automatically created header during configure
// It contains all the dynamic models declared in the config file
#include "Input/Dynamics/DynamicModels.h"

#include <iostream>

using namespace std;
using namespace LogFunctions;

extern Logger logger;
extern SimulatorConfiguration conf;

SimulAgent::SimulAgent() : pLayer(conf.GetSimulationTimeStep()), controller(nullptr)
{
}

SimulAgent::SimulAgent(const SimulAgent& a) : pLayer(a.pLayer)
{
	agent = a.agent;
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

void SimulAgent::SetState(const State& q)
{
    agent.SetState(q);
}

void SimulAgent::SetPossibleManeuvers(const ManeuverList& manList)
{
//FIXME	agent.SetPossibleManeuvers(manList);
}

bool SimulAgent::SetManeuver(const ManeuverName& manName)
{
// FIXME return agent.SetManeuver(manName);
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

Logger &operator<<(Logger &os, const SimulAgent &a)
{
    os << a.agent;
    return os;
}


void SimulAgent::EvolveState(const SensorOutput& sensorOutput, const double& currentTime)
{
	SendToController(sensorOutput, currentTime);
	Control control = controller->ComputeControl();
    agent.SetState(pLayer.GetNextState(agent.GetState(), control));
}

void SimulAgent::SendToController(const SensorOutput& sensorOutput, const double& currentTime)
{
	controller->ReceiveSensorOutput(sensorOutput, currentTime);
}


void SimulAgent::SetParameters(const AgentParameters& pars)
{
	agent.SetParameters(pars);
}

ExternalSensorOutput SimulAgent::RetrieveExternalSensorOutput(const std::string& sensorName, const Agent& selfInWorld, const AgentVector& othersInWorld, const EnvironmentParameters& envParams)
{
	const ExternalSensor* sensor = nullptr;
	for (auto itr = extSensors.begin(); itr != extSensors.end(); itr++)
	{
		if ((*itr)->GetName() == sensorName)
			sensor = *itr;
	}
	
	if (sensor == nullptr)
		Error("SimulAgent::RetrieveExternalSensorData", string("Sensor \'") + sensorName + "\'"  + "not found in SimulAgent sensor list");
	
	StateRegion visibleRegion;
	std::set<std::string> visibleIDs;
	
	sensor->SimulateVisibility(visibleRegion, visibleIDs, selfInWorld, othersInWorld);
	
	// Build vector of visible agents (with true states) and compute sensor output for those agents
	AgentVector trueVisibleAgents;
	for (auto visibleID = visibleIDs.begin(); visibleID != visibleIDs.end(); visibleID++)
		trueVisibleAgents[*visibleID] = othersInWorld.at(*visibleID);
	
	
	// ================ Prepare variables for to call sensor output =================
	ExternalSensor::SensorVars agentVars = sensor->GetMeasuredAgentVariables();
	State measuredState = State::GenerateStateOfType(agentVars);
	
	// measuredVisibleAgents will contain the output of the sensor
	AgentVector measuredVisibleAgents;
	for (auto visibleAgent = trueVisibleAgents.begin(); visibleAgent != trueVisibleAgents.end(); visibleAgent++)
	{
		// Create agent with true ID and empty state with vars that can be measured by this sensor
		Agent newAgent;
		newAgent.SetID(visibleAgent->second.GetID());
		newAgent.SetState(measuredState);
		
		measuredVisibleAgents[newAgent.GetID()] = newAgent;
	}
	
	// Now we do something similar for the environment measurements
	ExternalSensor::SensorVars envVars = sensor->GetMeasuredEnvironmentVariables();
	EnvironmentParameters measuredEnvParameters;
	
	for (auto itr = envVars.begin(); itr != envVars.end(); itr++)
		measuredEnvParameters.AddEntry(*itr, 0.0);
	
	// Compute the output
	sensor->SimulateOutput(measuredVisibleAgents, measuredEnvParameters, selfInWorld, othersInWorld, envParams);
	
	// Now build output object and return it
	ExternalSensorOutput output;
	output.SetVisibleRegion(visibleRegion);
	output.SetMeasuredAgents(measuredVisibleAgents);
	output.SetMeasuredEnvironment(measuredEnvParameters);
	
	return output;
	
}

InternalSensorOutput SimulAgent::RetrieveInternalSensorOutput(const std::string& sensorName, const Agent& trueSelfInWorld)
{
	const InternalSensor* sensor = nullptr;
	for (auto itr = intSensors.begin(); itr != intSensors.end(); itr++)
	{
		if ((*itr)->GetName() == sensorName)
			sensor = *itr;
	}
	
	if (sensor == nullptr)
		Error("SimulAgent::RetrieveInternalSensorData", string("Sensor \'") + sensorName + "\'"  + "not found in SimulAgent sensor list");
	
	
	// ================ Prepare variables for to call sensor output =================
	InternalSensor::SensorVars agentVars = sensor->GetSelfMeasuredVariables();
	State measuredSelfState = State::GenerateStateOfType(agentVars);
	
	Agent measuredSelf;
	measuredSelf.SetID(trueSelfInWorld.GetID());
	measuredSelf.SetState(measuredSelfState);
	
	// Compute the output
	sensor->SimulateOutput(measuredSelf, trueSelfInWorld);
	
	// Now build output object and return it
	InternalSensorOutput output;
	output.SetMeasuredSelf(measuredSelf);
	
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

