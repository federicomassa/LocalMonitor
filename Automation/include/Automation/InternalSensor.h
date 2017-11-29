#ifndef INTERNAL_SENSOR_H
#define INTERNAL_SENSOR_H

#include "Basic/Agent.h"


class InternalSensorOutput;

class InternalSensor
{
	typedef std::set<std::string> SensorVars;
	
	friend class InternalSensorPointer;
	std::string name;
 
	// Agent measurements (see world_agent_features in config file)
	SensorVars vars;	
	
protected:	
	void AddAgentMeasuredVariable(const std::string&);
	
public:
	virtual ~InternalSensor() {};
	
	// Agents are the same as the real agents but they are expressed in world variables (world_agent_features), via the StateConversion function declared in dynamic model 
	virtual void SimulateOutput( InternalSensorOutput &, const Agent& self) = 0;

};


class InternalSensorPointer
{
	const InternalSensor * s;
	
public:
	InternalSensorPointer (const std::string& name);
	~InternalSensorPointer();
	bool operator==(const InternalSensorPointer &) const;
	bool operator<(const InternalSensorPointer &) const;
	~InternalSensorPointer();
	InternalSensor * GetSensor();
	const InternalSensor * GetSensor() const;
	const std::string& GetName() const;
};

class InternalSensorOutput
{
	std::map<std::string, State> states;
public:
	State& operator()(const std::string&);
};


#endif
