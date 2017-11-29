#ifndef SENSOR_H
#define SENSOR_H

#include <string>
#include <vector>
#include "Basic/StateRegion.h"
#include "Basic/Agent.h"


class SensorOutput;
class SimulationParameters;

class ExternalSensor
{
	friend class SensorPointer;
	std::string name;
 
	typedef std::set<std::string> SensorVars;

	// Agent and environment measurements (see world_agent_features and world_environment_features in config file)
	SensorVars agentVars;	
	SensorVars envVars;
	void SetName(const std::string&);	

protected:
	
	void AddAgentMeasuredVariable(const std::string&);
	void AddEnvironmentMeasuredVariable(const std::string&);
	
public:
	ExternalSensor();
	virtual ~ExternalSensor();
	
	// Agents are the same as the real agents but they are expressed in world variables (world_agent_features), via the StateConversion function declared in dynamic model 
	virtual void SimulateVisibility(StateRegion& visibleRegion, std::set<std::string>& visibleIDs, const Agent& selfInWorld, const AgentVector& othersInWorld) const = 0;
	
	virtual void SimulateOutput(SensorOutput&, const Agent&, const AgentVector&, const SimulationParameters&) const = 0;
	
	
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
	std::map<std::string, State> states;
	std::map<std::string, double> envParams;
public:
	State& AgentID(const std::string&);
	double& Environment(const std::string&);
	
};

#endif
