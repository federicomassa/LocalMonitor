#include "NaiveEnvironment.h"
#include "Automation/ExternalSensor.h"
#include "Automation/InternalSensor.h"
#include "Utility/LogFunctions.h"
#include "NaiveObserver.h"
#include "Automation/Controller.h"
#include "Automation/PhysicalLayer.h"
#include "Automation/Automaton.h"
#include "Automation/Automatons/Automatons.h"
#include "Automation/Controllers/Controllers.h"
#include "Automation/Sensors/Sensors.h"
#include <string>

using namespace std;
using namespace LogFunctions;

NaiveEnvironment::NaiveEnvironment(NaiveObserver* parent, const Agent& s, const Maneuver& man, const AgentVector& o, const EnvironmentParameters& e, 
	const std::vector<std::shared_ptr<ExternalSensor> >& extSens,
	const std::vector<std::shared_ptr<InternalSensor> >& intSens, const bool& hidden) : observer(parent), extSensors(extSens), intSensors(intSens)
{
	SensorOutput output = SimulateSensors(s, o, e);
	
	// Populate map
	for (auto itr = o.begin(); itr != o.end(); itr++)
		localOthers[itr->first];
	
//	MyLogger logger(std::cout);
//	logger << localOthers << logger.EndL();
	
	output.RetrieveSensorData(self, others, env);
	selfManeuver = man;
	
	hasHiddenAgent = hidden;
	
	// Initialize pLayers ===============================
	for (auto itr = o.begin(); itr != o.end(); itr++)
	{
		try
		{
			pLayer[itr->first] = observer->pLayer(itr->first);
		}
		catch(out_of_range&)
		{
			pLayer[itr->first] = observer->pLayer("__default__");
		}
	}
	
	pLayer[observer->observedID] = observer->pLayer(observer->observedID);
	
	// ===================================================
	
	// == First set automaton and controller for observedID ====
	automaton = shared_ptr<Automaton>(InstantiateAutomaton(parent->controlModels(parent->observedID).GetAutomatonName()));
	
	controller[parent->observedID] = shared_ptr<Controller>(InstantiateController(parent->controlModels(parent->observedID).GetControllerName()));
	
	controller(parent->observedID)->SetControlModel(parent->controlModels(parent->observedID));
	
	// FIXME Time here is relative to beginning of prediction
	controller(parent->observedID)->ReceiveSensorOutput(output, 0);
	automaton->ReceiveSensorOutput(output, 0);
	// =========================================================
	
	
	// Now set controller for everyone else (simplistic model)
	for (auto itr = o.begin(); itr != o.end();
		 itr++)
		 {
			 
			// Use default value when not specified
			try
			{
				controller[itr->first] = shared_ptr<Controller>(InstantiateController(parent->controlModels(itr->first).GetControllerName()));
				
				controller(itr->first)->SetControlModel(parent->controlModels(itr->first));
			}
			catch(out_of_range&)
			{
				controller[itr->first] = shared_ptr<Controller>(InstantiateController(parent->controlModels("__default__").GetControllerName()));
				
				controller(itr->first)->SetControlModel(parent->controlModels("__default__"));
			}
			
			// Other agents only see themselves without further simulation. Their model should be pretty simple as they must only need this.
			SensorOutput noOthersOutput;
			noOthersOutput.SetSelf(others.at(itr->first));
			noOthersOutput.SetEnvironment(env);
			controller(itr->first)->ReceiveSensorOutput(noOthersOutput, 0);
		 }
}

NaiveEnvironment::~NaiveEnvironment()
{
}

NaiveEnvironment::NaiveEnvironment(const NaiveEnvironment& e) : extSensors(e.extSensors), intSensors(e.intSensors)
{
	observer = e.observer;
	
	self = e.self;
	selfManeuver = e.selfManeuver;
	others = e.others;
	
	localSelf = e.localSelf;
	localOthers = e.localOthers;
	
	env = e.env;
	hasHiddenAgent = e.hasHiddenAgent;
	
	pLayer = e.pLayer
	;
	automaton = e.automaton;
	controller = e.controller;
}

SensorOutput NaiveEnvironment::SimulateSensors(const Agent& trueSelfInWorld, const AgentVector& trueOthersInWorld, const EnvironmentParameters& trueEnvParams)
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

ExternalSensorOutput NaiveEnvironment::RetrieveExternalSensorOutput(const std::string& sensorName, const Agent& trueSelfInWorld, const AgentVector& trueOthersInWorld, const EnvironmentParameters& trueEnvParams)
{
	ExternalSensor* sensor = nullptr;
	for (auto itr = extSensors.begin(); itr != extSensors.end(); itr++)
	{
		if ((*itr)->GetName() == sensorName)
			sensor = itr->get();
	}
	
	if (sensor == nullptr)
		Error("NaiveEnvironment::RetrieveExternalSensorData", string("Sensor \'") + sensorName + "\'"  + "not found in SimulAgent sensor list");
	
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
	sensor->SimulateOutput(output, trueSelfInWorld, trueOthersInWorld, trueEnvParams);
		
	return output;
}


InternalSensorOutput NaiveEnvironment::RetrieveInternalSensorOutput(const std::string& sensorName, const Agent& trueSelfInWorld)
{
	const InternalSensor* sensor = nullptr;
	for (auto itr = intSensors.begin(); itr != intSensors.end(); itr++)
	{
		if ((*itr)->GetName() == sensorName)
			sensor = itr->get();
	}
	
	if (sensor == nullptr)
		Error("NaiveEnvironment::RetrieveInternalSensorData", string("Sensor \'") + sensorName + "\'"  + "not found in SimulAgent sensor list");
	
	
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

void NaiveEnvironment::Predict(const double& predictionSpan)
{
	double predictionTime = 0;
	double simulationStep = observer->pLayer.begin()->second.GetSimulationTimeStep();
	
	// =========== CONVERT WORLD TO STATE ============
	for (auto itr = pLayer.begin(); itr != pLayer.end(); itr++)
	{
		if (!hasHiddenAgent && itr->first == "__hidden__")
				continue;
		
		if (itr->first == observer->observedID)
		{
			localSelf = pLayer(itr->first).GetDynamicModel().GetLocalState(self,
																				   State::GenerateStateOfType(pLayer(itr->first).GetDynamicModel()));
		}
		else
		{			
			localOthers.at(itr->first) = pLayer(itr->first).GetDynamicModel().GetLocalState(others.at(itr->first),
																				   State::GenerateStateOfType(pLayer(itr->first).GetDynamicModel()));
		}
			
	}
	
	
	// ===============================================
	
	// Now run physical layer on local variables, then convert again to update agents expressed in world variables
	while (predictionTime < predictionSpan)
	{
		for (auto itr = controller.begin(); itr != controller.end(); itr++)
		{
			Control control;
			
			Maneuver currManeuver;
			if (itr->first == observer->observedID)
				currManeuver = selfManeuver;
			else
			{
				currManeuver = *(itr->second->GetControlModel().GetPossibleManeuvers().begin());
			}
			
			controller(itr->first)->ComputeControl(control, currManeuver);
						
			State q;
			
			if (itr->first == observer->observedID)
				q = pLayer(itr->first).GetNextState(localSelf, control);
			else
				q = pLayer(itr->first).GetNextState(localOthers.at(itr->first), control);
			
			
			if (itr->first == observer->observedID)
				localSelf.SetState(q);
			else
				localOthers.at(itr->first).SetState(q);
		}
		
		// Now re-convert to world and update measures (without sensor simulation)
		for (auto itr = pLayer.begin(); itr != pLayer.end(); itr++)
		{
			if (itr->first == observer->observedID)
			{
				State convertedState = itr->second.GetDynamicModel().GetWorldState(localSelf, State::GenerateStateOfType(observer->worldAgentVars));
				
				self.SetState(convertedState);
			}
			else
			{
				State convertedState = itr->second.GetDynamicModel().GetWorldState(localOthers.at(itr->first), State::GenerateStateOfType(observer->worldAgentVars));
				
				others.at(itr->first).SetState(convertedState);
			}
		}
		
		predictionTime += simulationStep;
		
		// Now update controllers and automaton
		for (auto itr = controller.begin(); itr != controller.end(); itr++)
		{
			SensorOutput output;
			if (itr->first == observer->observedID)
			{
				output.SetSelf(self);
				output.SetOthers(others);
				output.SetEnvironment(env);
				
				// Use updated time
				itr->second->ReceiveSensorOutput(output, predictionTime);
				automaton->ReceiveSensorOutput(output, predictionTime);
			}
			else
			{
				output.SetSelf(others.at(itr->first));
				output.SetEnvironment(env);
				
				// Use updated time and update controller only (there is no automaton)
				itr->second->ReceiveSensorOutput(output, predictionTime);
			}
		}
		
		
	}
}

const Agent & NaiveEnvironment::GetSelf() const
{
	return self;
}

const AgentVector & NaiveEnvironment::GetOthers() const
{
	return others;
}

const EnvironmentParameters & NaiveEnvironment::GetEnvironment() const
{
	return env;
}

const Maneuver & NaiveEnvironment::GetManeuver() const
{
	return selfManeuver;
}



