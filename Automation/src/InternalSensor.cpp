#include "InternalSensor.h"
#include "Input/Automation/Sensors.h"
#include "Utility/LogFunctions.h"

#include <string>

using namespace std;

void InternalSensor::AddSelfMeasuredVariable(const std::string& var)
{
	selfVars.insert(var);
}

const InternalSensor::SensorVars& InternalSensor::GetSelfMeasuredVariables() const
{
	return selfVars;
}

const std::string& InternalSensor::GetName()
{
	return name;
}

InternalSensorPointer::InternalSensorPointer(const std::string& name) : s(nullptr)
{
	s = InstantiateInternalSensor(name);
	
	LogFunctions::Require(s != nullptr, "InternalSensorPointer::InternalSensorPointer", string("Sensor ") + sens.name + " was not correctly instantiated.");
	
	s->name = name;
}

InternalSensorPointer::~InternalSensorPointer() 
{
	if (s)
		delete s;
}

InternalSensor* InternalSensorPointer::GetSensor()
{
	return s;
}

const InternalSensor* InternalSensorPointer::GetSensor() const
{
	return s;
}


const std::string& InternalSensorPointer::GetName() const
{
	return s->name;
}

void InternalSensorOutput::SetMeasuredSelf(const Agent& self)
{
	selfMeasure = self;
}

const Agent & InternalSensorOutput::GetMeasuredSelf() const
{
	return selfMeasure;
}
