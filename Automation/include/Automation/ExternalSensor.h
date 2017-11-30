#ifndef SENSOR_H
#define SENSOR_H

#include <string>
#include <vector>
#include "Basic/StateRegion.h"
#include "Basic/Agent.h"


class SensorOutput;
class EnvironmentParameters;

class ExternalSensor
{
	friend class SensorPointer;
	std::string name;
 

	// Agent and environment measurements (see world_agent_features and world_environment_features in config file)
	SensorVars agentVars;	
	SensorVars envVars;
	void SetName(const std::string&);	

protected:
	
	void AddAgentMeasuredVariable(const std::string&);
	void AddEnvironmentMeasuredVariable(const std::string&);
	
public:
	typedef std::set<std::string> SensorVars;

	ExternalSensor();
	virtual ~ExternalSensor();
	
	// Agents are the same as the real agents but they are expressed in world variables (world_agent_features), via the StateConversion function declared in dynamic model 
	virtual void SimulateVisibility(StateRegion& visibleRegion, std::set<std::string>& visibleIDs, const Agent& selfInWorld, const AgentVector& othersInWorld) const = 0;
	
	virtual void SimulateOutput(AgentVector& measuredAgents, EnvironmentParameters& measuredEnv, const Agent& trueSelf, const AgentVector& trueOthers, const EnvironmentParameters& trueEnvParams) const = 0;
	
	const SensorVars& GetMeasuredAgentVariables() const;
	const SensorVars& GetMeasuredEnvironmentVariables() const;
	
	const std::string& GetName() const;
	
};


class ExternalSensorPointer
{
	ExternalSensor* s;
	
public:
	ExternalSensorPointer (const std::string& name);
	bool operator==(const ExternalSensorPointer &) const;
	bool operator<(const ExternalSensorPointer &) const;
	~ExternalSensorPointer();
	ExternalSensor* GetSensor();
	const ExternalSensor* GetSensor() const;
	const std::string& GetName() const;
};

class ExternalSensorOutput
{
	AgentVector agentMeasuredStates;
	EnvironmentParameters envParams;
	StateRegion visibleRegion;
public:
	State& AgentID(const std::string&);
	double& Environment(const std::string&);
	
	void SetVisibleRegion(const StateRegion&);
	void SetMeasuredEnvironment(const EnvironmentParameters&);
	void SetMeasuredAgents(const AgentVector&);
	
	const StateRegion& GetVisibleRegion() const;
	const EnvironmentParameters& GetMeasuredEnvironment() const;
	const AgentVector& GetMeasuredAgents() const;
	
};

#endif
