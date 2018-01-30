#include "SensorOutput.h"
#include "Automation/ExternalSensor.h"
#include "Automation/InternalSensor.h"

#include <iostream>


using namespace std;

SensorOutput::SensorOutput()
{
}

// TODO check if maneuver is saved into sensor. Should be UNKNOWN to the observer
void SensorOutput::MergeExternalSensor(const ExternalSensorOutput& extOutput)
{
	// FIXME visible region is ignored
	AgentVector newAgentMeasurement = extOutput.GetMeasuredAgents();
	// ============ For each agent, merge sensor info
	for (auto agent = newAgentMeasurement.begin(); 
		 agent != newAgentMeasurement.end(); agent++)
		 {
			 State oldState = measuredAgents[agent->first].GetState();
			 const State& newState = agent->second.GetState();
			 
			 for (auto stateVar = newState.begin(); stateVar != newState.end(); stateVar++)
			 {
				// Check if merged sensor data already has this information
				try
				{
					// Info on this variable already present --- //FIXME take only last measure
					oldState(stateVar->first) = stateVar->second;
				}
				catch(out_of_range&)
				{
					// There was no info on this variable
					// Add it
					oldState.AddStateVariable(stateVar->first);
					oldState(stateVar->first) = stateVar->second;
				}
			 }
			 
			 // Update sensor data
			 measuredAgents[agent->first].SetState(oldState);
			 measuredAgents[agent->first].SetID(agent->first);

		 }
		 
	// =============== Now update environment
	EnvironmentParameters newEnvironmentMeasurement = extOutput.GetMeasuredEnvironment();

	for (auto envParam = newEnvironmentMeasurement.begin(); 
		 envParam != newEnvironmentMeasurement.end(); envParam++)
		 {
			// Check if merged sensor data already has this information
			try
			{
				// Info on this variable already present --- //FIXME take only last measure
				measuredEnvParams(envParam->first) = envParam->second;
			}
			catch(out_of_range&)
			{
				// There was no info on this variable
				// Add it
				measuredEnvParams.AddEntry(envParam->first, envParam->second);
			}
			
		 }		 
}

void SensorOutput::MergeInternalSensor(const InternalSensorOutput& intOutput)
{
	Agent newSelf = intOutput.GetMeasuredSelf();
	State oldState = selfMeasure.GetState();
	
	for (auto stateVar = newSelf.GetState().begin(); stateVar != newSelf.GetState().end(); stateVar++)
		{
			// Check if merged sensor data already has this information
			try
			{
				// Info on this variable already present --- //FIXME take only last measure
				oldState(stateVar->first) = stateVar->second;
			}
			catch(out_of_range&)
			{
				// There was no info on this variable
				// Add it
				oldState.AddStateVariable(stateVar->first);
				oldState(stateVar->first) = stateVar->second;
			}
		}
		
	// Update
	selfMeasure.SetState(oldState);
	selfMeasure.SetID(newSelf.GetID());
}

void SensorOutput::RetrieveSensorData(Agent& self, AgentVector& others, EnvironmentParameters& env)
{
	self = selfMeasure;
	others = measuredAgents;
	env = measuredEnvParams;
}

const Agent & SensorOutput::RetrieveSelfData() const
{
	return selfMeasure;
}

const AgentVector & SensorOutput::RetrieveOtherAgentsData() const
{
	return measuredAgents;
}

const EnvironmentParameters & SensorOutput::RetrieveEnvironmentData() const
{
	return measuredEnvParams;
}

void SensorOutput::SetSelf(const Agent& a)
{
	selfMeasure = a;
}

void SensorOutput::SetOthers(const AgentVector& o)
{
	measuredAgents = o;
}

void SensorOutput::SetEnvironment(const EnvironmentParameters& e)
{
	measuredEnvParams = e;
}





