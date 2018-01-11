#ifndef SIMUL_AGENT_H
#define SIMUL_AGENT_H

#include "Basic/Agent.h"
#include "Automation/DynamicModel.h"
#include "Automation/PhysicalLayer.h"
#include "Automation/Controller.h"
#include "Automation/Automaton.h"
#include "Automation/InternalSensor.h"
#include "Automation/ExternalSensor.h"
#include "Automation/SensorOutput.h"
#include "Utility/AgentParameters.h"

#include <string>
#include <map>
#include <set>
#include <memory>

class MyLogger;
class SimulatorConfiguration;

class SimulAgent
{
    friend class SimulatorConfiguration;

    Agent agent;
    PhysicalLayer pLayer;
	Controller* controller;
	Automaton* automaton;
	// converted state to world coordinates. Set by GenerateWorldState
	State worldState;
	
	SimulatorConfiguration* conf;
	// Sensors
	std::vector<std::shared_ptr<ExternalSensor> > extSensors;
	std::vector<std::shared_ptr<InternalSensor> > intSensors;
	
	ExternalSensorOutput RetrieveExternalSensorOutput(const std::string& sensorName,
		const Agent& trueSelfInWorld, const AgentVector& trueOthersInWorld, 
		const EnvironmentParameters& trueEnvParams);
	
	InternalSensorOutput RetrieveInternalSensorOutput(const std::string& sensorName,
		const Agent& trueSelfInWorld);
	
	
	void SendToController(const SensorOutput&, const double& currentTime);
	void SendToAutomaton(const SensorOutput&, const double& currentTime);

	
public:
    SimulAgent(SimulatorConfiguration* conf = nullptr);
	SimulAgent(const SimulAgent&);
	~SimulAgent();
	SimulAgent& operator=(const SimulAgent&);
    const std::string &GetID() const;
    void SetID ( const std::string & );
    const State &GetState() const;
	const Maneuver& GetManeuver() const;
	const DynamicModel& GetDynamicModel() const;
    void SetState ( const State & );
	void SetDynamicModel(const DynamicModel&);
	void SetParameters(const AgentParameters&);
	const AgentParameters& GetParameters() const;
	State GenerateWorldState(const State&);
	const State& GetWorldState() const;
	const Agent& GetAgent() const;
    void Run(const SensorOutput&, const double& currentTime);
	
	SensorOutput SimulateSensors(const Agent& trueSelfInWorld, const AgentVector& trueOthersInWorld, const EnvironmentParameters& trueEnvParams);
	
    friend MyLogger &operator<< ( MyLogger &, const SimulAgent & );
};

typedef std::map<std::string, SimulAgent> SimulAgentVector;

MyLogger &operator<< ( MyLogger &, const SimulAgent & );

#endif
