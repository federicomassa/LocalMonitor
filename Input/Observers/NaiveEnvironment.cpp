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
#include <iomanip>

using namespace std;
using namespace LogFunctions;

NaiveEnvironment::NaiveEnvironment(NaiveObserver *parent, const Agent &s, const Maneuver &man, const AgentVector &o, const EnvironmentParameters &e,
                                   const std::vector<std::shared_ptr<ExternalSensor> > &extSens,
                                   const std::vector<std::shared_ptr<InternalSensor> > &intSens, const bool &hidden) : observer(parent), extSensors(&extSens), intSensors(&intSens)
{
    trueSelf = s;
    trueOthers = o;
    trueEnv = e;

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
    for (auto itr = o.begin(); itr != o.end(); itr++) {
        try {
            pLayer[itr->first] = observer->pLayer(itr->first);
        } catch (out_of_range &) {
            pLayer[itr->first] = observer->pLayer("__default__");
        }
    }

    pLayer[observer->GetObservedID()] = observer->pLayer(observer->GetObservedID());

    // ===================================================

    // == First set automaton and controller for GetObservedID() ====
    automaton = shared_ptr<Automaton>(InstantiateAutomaton(parent->controlModels(parent->GetObservedID()).GetAutomatonName()));
	
    controller[parent->GetObservedID()] = shared_ptr<Controller>(InstantiateController(parent->controlModels(parent->GetObservedID()).GetControllerName()));

    controller(parent->GetObservedID())->SetControlModel(parent->controlModels(parent->GetObservedID()));

    controller(parent->GetObservedID())->ReceiveSensorOutput(output, observer->lastPredictionStartTime);
    automaton->ReceiveSensorOutput(output, observer->lastPredictionStartTime);
    // =========================================================


    // Now set controller for everyone else (simplistic model)
    for (auto itr = o.begin(); itr != o.end();
            itr++) {

        // Use default value when not specified
        try {
            controller[itr->first] = shared_ptr<Controller>(InstantiateController(parent->controlModels(itr->first).GetControllerName()));

            controller(itr->first)->SetControlModel(parent->controlModels(itr->first));
        } catch (out_of_range &) {
            controller[itr->first] = shared_ptr<Controller>(InstantiateController(parent->controlModels("__default__").GetControllerName()));

            controller(itr->first)->SetControlModel(parent->controlModels("__default__"));
        }

        // Other agents only see themselves without further simulation. Their model should be pretty simple as they must only need this.
        SensorOutput othersOutput;
        othersOutput.SetSelf(others.at(itr->first));
        othersOutput.SetEnvironment(env);

        // build others
        AgentVector otherNeighbors;
        for (auto agent = o.begin(); agent != o.end(); agent++)
            if (agent->second.GetID() != itr->first)
                otherNeighbors[agent->first] = agent->second;

        otherNeighbors[itr->first] = itr->second;

        othersOutput.SetOthers(otherNeighbors);

        controller(itr->first)->ReceiveSensorOutput(othersOutput, observer->lastPredictionStartTime);
    }

    // Use controller as container only because everyone has one (need IDs)
    bool writeOutput = !hasHiddenAgent && observer->lastPredictionStartTime == 0 &&
    selfManeuver == "sigma_1";
    
	if (writeOutput)
	{
		for (auto itr = controller.begin(); itr != controller.end(); itr++) 
		{
			dataOutput[itr->first] = shared_ptr<std::ofstream>(new ofstream((string("PredictionDataOutput") + itr->first + ".txt").c_str()));

			if (dataOutput(itr->first)->is_open()) {
				dataLogger[itr->first].SetOutput(dataOutput(itr->first).get());
				dataLogger(itr->first).SetPrecision(15);
			} else
				LogFunctions::Error("NaiveEnvironment::NaiveEnvironment", string("Error opening file \'PredictionDataOutput") + itr->first + ".txt\'");
		}
	}


}

NaiveEnvironment::~NaiveEnvironment()
{
}

NaiveEnvironment::NaiveEnvironment(const NaiveEnvironment &e)
{
    *this = e;
}

const NaiveEnvironment &NaiveEnvironment::operator=(const NaiveEnvironment &e)
{
    extSensors = e.extSensors;
    intSensors = e.intSensors;

    observer = e.observer;

    self = e.self;
    selfManeuver = e.selfManeuver;
    others = e.others;

    localSelf = e.localSelf;
    localOthers = e.localOthers;

    env = e.env;
    hasHiddenAgent = e.hasHiddenAgent;

    pLayer = e.pLayer;
    automaton = e.automaton;
    controller = e.controller;

	dataOutput = e.dataOutput;
	dataLogger = e.dataLogger;
	
    return *this;
}


SensorOutput NaiveEnvironment::SimulateSensors(const Agent &trueSelfInWorld, const AgentVector &trueOthersInWorld, const EnvironmentParameters &trueEnvParams)
{
    SensorOutput sensorOutput;

    // Call external external sensors
    for (auto extSensor = extSensors->begin(); extSensor != extSensors->end(); extSensor++) {
        ExternalSensorOutput extOutput = RetrieveExternalSensorOutput((*extSensor)->GetName(), trueSelfInWorld, trueOthersInWorld, trueEnvParams);

        sensorOutput.MergeExternalSensor(extOutput);
    }

    // Call internal sensors
    for (auto intSensor = intSensors->begin(); intSensor != intSensors->end(); intSensor++) {
        InternalSensorOutput intOutput = RetrieveInternalSensorOutput((*intSensor)->GetName(), trueSelfInWorld);

        sensorOutput.MergeInternalSensor(intOutput);
    }

    return sensorOutput;

}

ExternalSensorOutput NaiveEnvironment::RetrieveExternalSensorOutput(const std::string &sensorName, const Agent &trueSelfInWorld, const AgentVector &trueOthersInWorld, const EnvironmentParameters &trueEnvParams)
{
    ExternalSensor *sensor = nullptr;
    for (auto itr = extSensors->begin(); itr != extSensors->end(); itr++) {
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
    for (auto visibleID = visibleIDs.begin(); visibleID != visibleIDs.end(); visibleID++) {
        trueVisibleAgents[*visibleID] = trueOthersInWorld.at(*visibleID);
    }

    // ================ Prepare variables for to call sensor output =================
    ExternalSensor::SensorVars agentVars = sensor->GetMeasuredAgentVariables();

    State measuredState = State::GenerateStateOfType(agentVars);

    // measuredVisibleAgents will contain the output of the sensor
    AgentVector measuredVisibleAgents;
    for (auto visibleAgent = trueVisibleAgents.begin(); visibleAgent != trueVisibleAgents.end(); visibleAgent++) {
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


InternalSensorOutput NaiveEnvironment::RetrieveInternalSensorOutput(const std::string &sensorName, const Agent &trueSelfInWorld)
{
    const InternalSensor *sensor = nullptr;
    for (auto itr = intSensors->begin(); itr != intSensors->end(); itr++) {
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

void NaiveEnvironment::Predict(const double &predictionSpan)
{
    double predictionTime = observer->lastPredictionStartTime;
    double simulationStep = observer->pLayer.begin()->second.GetSimulationTimeStep();

    bool writeOutput = !hasHiddenAgent && observer->lastPredictionStartTime == 0 && GetManeuver() == "sigma_1";


    if (writeOutput) {
        std::cout << "OPENING" << std::endl;
    }


    // =========== CONVERT WORLD TO STATE ============
    for (auto itr = pLayer.begin(); itr != pLayer.end(); itr++) {
        if (!hasHiddenAgent && itr->first == "__hidden__")
            continue;

        if (itr->first == observer->GetObservedID()) {
            localSelf = pLayer(itr->first).GetDynamicModel().GetLocalState(self,
                        State::GenerateStateOfType(pLayer(itr->first).GetDynamicModel()));
        } else {
            localOthers.at(itr->first) = pLayer(itr->first).GetDynamicModel().GetLocalState(others.at(itr->first),
                                         State::GenerateStateOfType(pLayer(itr->first).GetDynamicModel()));
        }

    }


    // Write output
    if (writeOutput) {

        // Use controller to take all IDs
        for (auto control = controller.begin(); control != controller.end(); control++) {
            dataLogger(control->first) << "Timestamp" << '\t';

            for (auto var = self.GetState().begin(); var != self.GetState().end(); var++) {
                dataLogger(control->first) << var->first << '\t';
            }


            for (auto controlVar = control->second->GetControlModel().GetControlVariables().begin();
                    controlVar != control->second->GetControlModel().GetControlVariables().end();
                    controlVar++) {
                dataLogger(control->first) << *controlVar << '\t';
            }

            for (auto man = control->second->GetControlModel().GetPossibleManeuvers().begin();
                    man != control->second->GetControlModel().GetPossibleManeuvers().end();
                    man++) {
                dataLogger(control->first) << man->GetName() << '\t';
            }

            dataLogger(control->first) << dataLogger(control->first).EndL();

        }

    }

    // ===============================================

    // Now run physical layer on local variables, then convert again to update agents expressed in world variables
    while (predictionTime < observer->lastPredictionStartTime + predictionSpan) {

        if (writeOutput) {
            // Use controller container to get all IDs at once
            for (auto control = controller.begin(); control != controller.end(); control++) {
                //Timestamp
                dataLogger(control->first) << predictionTime << '\t';

                // Continuous state
                for (auto itr = self.GetState().begin(); itr != self.GetState().end(); itr++) {
                    // If agent is the observed one, use self Agent, otherwise use others
                    if (control->first == self.GetID())
                        dataLogger(control->first) << itr->second << '\t';
                    else
                        dataLogger(control->first) << others.at(control->first)(itr->first) << '\t';
                }

                // Controls (first step will be zero because control is referred to previous time step)
                for (auto itr = control->second->GetControlModel().GetControlVariables().begin();
                        itr != control->second->GetControlModel().GetControlVariables().end();
                        itr++) {
						dataLogger(control->first) << control->second->GetLastControl()(*itr) << '\t';
					}

                // Maneuvers
                for (auto itr = control->second->GetControlModel().GetPossibleManeuvers().begin(); itr != control->second->GetControlModel().GetPossibleManeuvers().end(); itr++) {
                    if (control->first == self.GetID())
                        dataLogger(control->first) << (automaton->GetManeuver() == *itr) << '\t';
                    else
                        // WARNING
                        dataLogger(control->first) << 1;
                }

                dataLogger(control->first) << dataLogger(control->first).EndL();
            }
        }

		IMap<State> newQ;

        for (auto itr = controller.begin(); itr != controller.end(); itr++) {
            Control control;

			newQ[itr->first];
            Maneuver currManeuver;
            if (itr->first == observer->GetObservedID())
                currManeuver = selfManeuver;
            else {
                // WARNING Select first (and only?) maneuver in control model
                currManeuver = *(itr->second->GetControlModel().GetPossibleManeuvers().begin());
            }

            controller(itr->first)->ComputeControl(control, currManeuver);

            // FIXME Now we are using simulated others, this is only --almost-- right
            // because physical layer should have the true values, which should be
            // simulated separately and then filtered by sensors each time
            // Instead, now we are using simulated data at first and then
            // predicting without further simulation, which should be
            // done at each prediction step

            if (itr->first == observer->GetObservedID())
                newQ(itr->first) = pLayer(itr->first).GetNextState(localSelf, others, control);
            else {
                //Build neighbors
                AgentVector otherNeighbors;

                // Add self as neighbor
                otherNeighbors[localSelf.GetID()] = localSelf;

                // Add others except itself
                for (auto agent = localOthers.begin(); agent != localOthers.end(); agent++)
                    if (agent->first != itr->first)
                        otherNeighbors[agent->first] = agent->second;

                newQ(itr->first) = pLayer(itr->first).GetNextState(localOthers.at(itr->first),
                                                    otherNeighbors, control);

            }

        }
        
        for (auto itr = controller.begin(); itr != controller.end(); itr++)
		{
          if (itr->first == observer->GetObservedID())
                localSelf.SetState(newQ(itr->first));
            else
                localOthers.at(itr->first).SetState(newQ(itr->first));
		}
			
        // Now re-convert to world and update measures (without sensor simulation)
        for (auto itr = pLayer.begin(); itr != pLayer.end(); itr++) {
            if (itr->first == observer->GetObservedID()) {
                State convertedState = itr->second.GetDynamicModel().GetWorldState(localSelf, State::GenerateStateOfType(observer->worldAgentVars));

                self.SetState(convertedState);
            } else {
                State convertedState = itr->second.GetDynamicModel().GetWorldState(localOthers.at(itr->first), State::GenerateStateOfType(observer->worldAgentVars));

                others.at(itr->first).SetState(convertedState);
            }
        }

        predictionTime += simulationStep;
	
        // Now update controllers and automaton
        for (auto itr = controller.begin(); itr != controller.end(); itr++) {
            SensorOutput output;
            if (itr->first == observer->GetObservedID()) {
                output.SetSelf(self);
                output.SetOthers(others);
                output.SetEnvironment(env);

                // Use updated time
                itr->second->ReceiveSensorOutput(output, predictionTime);
                automaton->ReceiveSensorOutput(output, predictionTime);
            } else {
                output.SetSelf(others.at(itr->first));
                output.SetEnvironment(env);

                // Build neighbors for other agents (not observed ones)
                AgentVector otherNeighbors;
                otherNeighbors[self.GetID()] = self;

                for (auto agent = others.begin(); agent != others.end(); agent++)
                    if (agent->first != itr->first)
                        otherNeighbors[agent->first] = agent->second;

                observer->logger << "I am " << itr->first << " and I have " << otherNeighbors.size() << " neighbors:" << otherNeighbors << observer->logger.EndL();

                output.SetOthers(otherNeighbors);

                // WARNING Use updated time and update controller only (there is no automaton)
                itr->second->ReceiveSensorOutput(output, predictionTime);
            }
        }


    }
}

const Agent &NaiveEnvironment::GetSelf() const
{
    return self;
}

const Agent &NaiveEnvironment::GetLocalSelf() const
{
    return localSelf;
}

const AgentVector &NaiveEnvironment::GetOthers() const
{
    return others;
}

const EnvironmentParameters &NaiveEnvironment::GetEnvironment() const
{
    return env;
}

const Maneuver &NaiveEnvironment::GetManeuver() const
{
    return selfManeuver;
}

const bool &NaiveEnvironment::HasHiddenAgent() const
{
    return hasHiddenAgent;
}

