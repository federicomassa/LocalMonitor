#include "InternalSensor.h"
#include "Input/Automation/Sensors.h"
#include "Utility/LogFunctions.h"

#include <string>

using namespace std;

void InternalSensor::AddAgentMeasuredVariable(const std::string& var)
{
	selfVars.insert(var);
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
