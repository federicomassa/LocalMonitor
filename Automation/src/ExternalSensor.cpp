#include "ExternalSensor.h"
#include "Input/Automation/Sensors.h"
#include "Utility/LogFunctions.h"

#include <string>

using namespace std;

ExternalSensor::ExternalSensor()
{}

ExternalSensor::~ExternalSensor()
{}

void ExternalSensor::AddAgentMeasuredVariable(const std::string& var)
{
	agentVars.insert(var);
}

void ExternalSensor::AddEnvironmentMeasuredVariable(const std::string& var)
{
	envVars.insert(var);
}


void ExternalSensor::SetName(const string& n)
{
	name = n;
}

const std::string& ExternalSensor::GetName() const
{
	return name;
}

void ExternalSensor::SetType(const SensorType& t)
{
	type = t;
}

ExternalSensorPointer::ExternalSensorPointer(const std::string& name) : s(nullptr) 
{
	s = InstantiateExternalSensor(name);
	
	LogFunctions::Require(s != nullptr, "ExternalSensorPointer::ExternalSensorPointer", string("Sensor ") + name + " was not correctly instantiated.");
	
	s->name = name;
}

ExternalSensor * ExternalSensorPointer::GetSensor()
{
	return s;
}

const ExternalSensor * ExternalSensorPointer::GetSensor() const
{
	return s;
}

const std::string& ExternalSensorPointer::GetName() const
{
	return s->name;
}

ExternalSensorPointer::~ExternalSensorPointer() 
{
	if (s)
		delete s;
}

bool ExternalSensorPointer::operator==(const ExternalSensorPointer & sp) const
{
	return (s->name == sp.s->name);
}

bool ExternalSensorPointer::operator<(const ExternalSensorPointer & sp) const
{
	return (s->name < sp.s->name);
}


State& ExternalSensorOutput::AgentID(const std::string& id)
{
	return states.at(id);
}

double& ExternalSensorOutput::Environment(const std::string& envParameter)
{
	return envParams.at(envParameter);
}
