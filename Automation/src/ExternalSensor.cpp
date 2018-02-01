#include "ExternalSensor.h"
//#include "Input/Automation/Sensors/Sensors.h"
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

const ExternalSensor::SensorVars & ExternalSensor::GetMeasuredAgentVariables() const
{
	return agentVars;
}


const ExternalSensor::SensorVars & ExternalSensor::GetMeasuredEnvironmentVariables() const
{
	return envVars;
}

/*
ExternalSensor::Pointer(const std::string& name) : s(nullptr) 
{
	s = InstantiateExternalSensor(name);
	
	LogFunctions::Require(s != nullptr, "ExternalSensor::Pointer::ExternalSensor::Pointer", string("Sensor ") + name + " was not correctly instantiated.");
	
	s->name = name;
}

ExternalSensor::Pointer(const ExternalSensor::Pointer& ep) : s(nullptr) 
{
	s = InstantiateExternalSensor(ep.s->GetName());
	
	LogFunctions::Require(s != nullptr, "ExternalSensor::Pointer::ExternalSensor::Pointer", string("Sensor ") + ep.s->GetName() + " was not correctly instantiated.");
	
	s->name = ep.s->GetName();
}

ExternalSensor::Pointer& ExternalSensor::Pointer::operator=(const ExternalSensor::Pointer& ep)
{
	if (s)
		delete s;
	
	s = InstantiateExternalSensor(ep.s->GetName());
	
	LogFunctions::Require(s != nullptr, "ExternalSensor::Pointer::ExternalSensor::Pointer", string("Sensor ") + ep.s->GetName() + " was not correctly instantiated.");
	
	s->name = ep.s->GetName();
}


ExternalSensor * ExternalSensor::Pointer::GetSensor()
{
	return s;
}

const ExternalSensor * ExternalSensor::Pointer::GetSensor() const
{
	return s;
}

const std::string& ExternalSensor::Pointer::GetName() const
{
	return s->name;
}

ExternalSensor::Pointer::~Pointer() 
{
	if (s)
		delete s;
}

bool ExternalSensor::Pointer::operator==(const ExternalSensor::Pointer & sp) const
{
	return (s->name == sp.s->name);
}

bool ExternalSensor::Pointer::operator<(const ExternalSensor::Pointer & sp) const
{
	return (s->name < sp.s->name);
}
*/


void ExternalSensorOutput::SetMeasuredEnvironment(const EnvironmentParameters& e)
{
	envParams = e;
}


void ExternalSensorOutput::SetMeasuredAgents(const AgentVector& agents)
{
	agentMeasuredStates = agents;
}

const AgentVector & ExternalSensorOutput::GetMeasuredAgents() const
{
	return agentMeasuredStates;
}


void ExternalSensorOutput::SetVisibleRegion(const StateRegion& vis)
{
	visibleRegion = vis;
}

const StateRegion & ExternalSensorOutput::GetVisibleRegion() const
{
	return visibleRegion;
}


State & ExternalSensorOutput::AgentID(const std::string& ID)
{	
	return agentMeasuredStates.at(ID).state;
}

double & ExternalSensorOutput::Environment(const std::string& param)
{
	return envParams(param);
}

const EnvironmentParameters & ExternalSensorOutput::GetMeasuredEnvironment() const
{
	return envParams;
}

