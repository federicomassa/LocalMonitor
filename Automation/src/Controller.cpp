#include "Controller.h"
#include <iostream>

Controller::Controller(const std::string& n)
{
	name = n;
}

// Default functionality, stores only most current data
void Controller::ReceiveSensorOutput(const SensorOutput& sensorOutput, const double& currentTime)
{
	std::cout << "Here?" << std::endl;
	
	const Agent& currentSelf = sensorOutput.RetrieveSelfData();
	const AgentVector& currentOthers = sensorOutput.RetrieveOtherAgentsData();
	const EnvironmentParameters& currentEnv = sensorOutput.RetrieveEnvironmentData();
	
	std::cout << "Here2?" << std::endl;
	
	selfTrajectory.clear();
	othersTrajectory.clear();
	environmentTrajectory.clear();
	
	std::cout << "Here3?" << std::endl;
	
	selfTrajectory.insert(currentTime, currentSelf);
	othersTrajectory.insert(currentTime, currentOthers);
	environmentTrajectory.insert(currentTime, currentEnv);
	
	std::cout << "Here4?" << std::endl;
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
