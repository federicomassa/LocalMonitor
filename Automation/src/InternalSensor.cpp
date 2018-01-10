#include "InternalSensor.h"
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

const std::string& InternalSensor::GetName() const
{
	return name;
}

/*
InternalSensorPointer::InternalSensorPointer(const std::string& name) : s(nullptr)
{
	s = InstantiateInternalSensor(name);
	
	LogFunctions::Require(s != nullptr, "InternalSensorPointer::InternalSensorPointer", string("Sensor ") + name + " was not correctly instantiated.");
	
	s->name = name;
}

InternalSensorPointer::InternalSensorPointer(const InternalSensorPointer& ip)
{
	const string& name = ip.s->GetName();
	s = InstantiateInternalSensor(name);
	
	LogFunctions::Require(s != nullptr, "InternalSensorPointer::InternalSensorPointer", string("Sensor ") + name + " was not correctly instantiated.");
	
	s->name = name;	
}

 
InternalSensorPointer& InternalSensorPointer::operator=(const InternalSensorPointer& ip)
{
	if (s)
		delete s;
	
	const string& name = ip.s->GetName();
	s = InstantiateInternalSensor(name);
	
	LogFunctions::Require(s != nullptr, "InternalSensorPointer::InternalSensorPointer", string("Sensor ") + name + " was not correctly instantiated.");
	
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


bool InternalSensorPointer::operator<(const InternalSensorPointer& p) const
{
	return (GetName() < p.GetName());
}

bool InternalSensorPointer::operator==(const InternalSensorPointer& p) const
{
	return (GetName() == p.GetName());
}
*/

double & InternalSensorOutput::operator()(const std::string& varName)
{
	return selfMeasure(varName);
}


void InternalSensorOutput::SetMeasuredSelf(const Agent& self)
{
	selfMeasure = self;
}

const Agent & InternalSensorOutput::GetMeasuredSelf() const
{
	return selfMeasure;
}
