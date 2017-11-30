#ifndef INTERNAL_SENSOR_H
#define INTERNAL_SENSOR_H

#include "Basic/Agent.h"


class InternalSensorOutput;

class InternalSensor
{
	
	friend class InternalSensorPointer;
	std::string name;
 
	// Agent measurements (see world_agent_features in config file)
	SensorVars vars;	
	
protected:	
	void AddSelfMeasuredVariable(const std::string&);
	
public:
	typedef std::set<std::string> SensorVars;

	virtual ~InternalSensor() {};
	
	// Agents are the same as the real agents but they are expressed in world variables (world_agent_features), via the StateConversion function declared in dynamic model 
	virtual void SimulateOutput( Agent& measuredSelf, const Agent& trueSelf) const = 0;
	const SensorVars& GetSelfMeasuredVariables() const;
	const std::string& GetName() const;
	
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
	Agent selfMeasure;
public:
	void SetMeasuredSelf(const Agent&);
	const Agent& GetMeasuredSelf() const;
};


#endif