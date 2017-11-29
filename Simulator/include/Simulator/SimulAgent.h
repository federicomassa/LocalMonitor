#ifndef SIMUL_AGENT_H
#define SIMUL_AGENT_H

#include "Basic/Agent.h"
#include "Automation/DynamicModel.h"
#include "Automation/AutomationModel.h"
#include "Automation/PhysicalLayer.h"
#include "Automation/Controller.h"
#include "Automation/Automaton.h"
#include "Automation/InternalSensor.h"
#include "Automation/ExternalSensor.h"

#include <string>
#include <map>
#include <set>

class Logger;


class SimulAgent
{
    friend class SimulatorConfiguration;

    Agent agent;
    PhysicalLayer pLayer;
	Controller controller;
	Automaton automaton;
	
	// converted state to world coordinates. Set by GenerateWorldState
	State worldState;
	AutomationModel automationModel;
	
	// NB now we use Sensor*, not SensorPointer, because we want only
	// one real instance of a sensor (instantiated in SensorPointer), and
	// here we use references to the sensor inside SensorPointer
	std::set<const ExternalSensor *> extSensors;
	std::set<const InternalSensor *> intSensors;
	
	std::map<std::string, ExternalSensorOutput> RetrieveExternalSensorData(const std::string& sensorName);
	InternalSensorOutput RetrieveInternalSensorOutput(const std::string& sensorName);
	
	void SetDynamicFunction ( const std::string & );
public:
    SimulAgent();
	SimulAgent(const SimulAgent&);
    const std::string &GetID() const;
    void SetID ( const std::string & );
    const State &GetState() const;
	const DynamicModel& GetDynamicModel() const;
    void SetState ( const State & );
	void SetPossibleManeuvers(const ManeuverList&);
	void SetDynamicModel(const DynamicModel&);
	void SetParameters(const AgentParameters&);
	State GenerateWorldState(const State&);
	const State& GetWorldState() const;
	const Agent& GetAgent() const;
	bool SetManeuver(const ManeuverName&);
    void EvolveState();
    friend Logger &operator<< ( Logger &, const SimulAgent & );
};

typedef std::map<std::string, SimulAgent> SimulAgentVector;

Logger &operator<< ( Logger &, const SimulAgent & );

#endif
