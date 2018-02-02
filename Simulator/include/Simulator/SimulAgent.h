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
#include "Observer/LocalMonitor.h"

#include <string>
#include <map>
#include <set>
#include <memory>

class MyLogger;
class SimulatorConfiguration;
class Environment;

class SimulAgent
{
    friend class SimulatorConfiguration;

	const Environment* environment;
	
    Agent agent;
    PhysicalLayer pLayer;
	Controller* controller;
	Automaton* automaton;
	LocalMonitor localMonitor;
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
	void SendToLocalMonitor(const SensorOutput&, const double& currentTime);
	
public:
    SimulAgent(SimulatorConfiguration* conf = nullptr);
	SimulAgent(const SimulAgent&);
	~SimulAgent();
	SimulAgent& operator=(const SimulAgent&);
    const std::string &GetID() const;
    void SetID ( const std::string & );
    const State &GetState() const;
	const Maneuver& GetManeuver() const;
	const Automaton* GetAutomaton() const;
	const Controller* GetController() const;
	const DynamicModel& GetDynamicModel() const;
    void SetState ( const State & );
	void SetDynamicModel(const DynamicModel&);
	void SetParameters(const AgentParameters&);
	void SetEnvironment(const Environment*);
	const AgentParameters& GetParameters() const;
	State GenerateWorldState(const State&);
	const State& GetWorldState() const;
	const Agent& GetAgent() const;
    void Run(const SensorOutput&, const double& currentTime);
	void InitializeLocalMonitor(const std::string& configFilePath);
	
	SensorOutput SimulateSensors(const Agent& trueSelfInWorld, const AgentVector& trueOthersInWorld, const EnvironmentParameters& trueEnvParams);
	
    friend MyLogger &operator<< ( MyLogger &, const SimulAgent & );
};

typedef std::map<std::string, SimulAgent> SimulAgentVector;

MyLogger &operator<< ( MyLogger &, const SimulAgent & );

#endif
