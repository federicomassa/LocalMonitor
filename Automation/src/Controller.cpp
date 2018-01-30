#include "Controller.h"
#include <iostream>

Controller::Controller(const std::string& n)
{
	name = n;
}

// Default functionality, stores only current data
void Controller::ReceiveSensorOutput(const SensorOutput& sensorOutput, const double& currentTime)
{
	const Agent& currentSelf = sensorOutput.RetrieveSelfData();
	const AgentVector& currentOthers = sensorOutput.RetrieveOtherAgentsData();
	const EnvironmentParameters& currentEnv = sensorOutput.RetrieveEnvironmentData();
	
	selfTrajectory.clear();
	othersTrajectory.clear();
	environmentTrajectory.clear();
	
	selfTrajectory.insert(currentTime, currentSelf);
	othersTrajectory.insert(currentTime, currentOthers);
	environmentTrajectory.insert(currentTime, currentEnv);
}

const TimedContainer<Agent> & Controller::GetSelfTrajectory() const
{
	return selfTrajectory;
}

const TimedContainer<AgentVector> & Controller::GetOtherAgentsTrajectory() const
{
	return othersTrajectory;
}

const TimedContainer<EnvironmentParameters> & Controller::GetEnvironmentTrajectory() const
{
	return environmentTrajectory;
}

Controller::~Controller() {}

const std::string & Controller::GetName() const
{
	return name;
}

void Controller::SetControlModel(const ControlModel& mod)
{
	model = mod;
}

const ControlModel & Controller::GetControlModel() const
{
	return model;
}

Controller::Controller(const Controller& c)
{
	*this = c;
}

const Controller & Controller::operator=(const Controller& c)
{
	name = c.name;
	selfTrajectory = c.selfTrajectory;
	othersTrajectory = c.othersTrajectory;
	environmentTrajectory = c.environmentTrajectory;
	
	model = c.model;
	
	return *this;
}



