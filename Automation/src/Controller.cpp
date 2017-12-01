#include "Controller.h"

// Default functionality, stores only most current data
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



