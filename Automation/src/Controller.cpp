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
	lastControl = State::GenerateStateOfType(model.GetControlVariables());
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
	failures = c.failures;
	
	model = c.model;
	
	return *this;
}

void Controller::SaveControl(const Control& u)
{
	lastControl = u;
}

const Control & Controller::GetLastControl() const
{
	return lastControl;
}

void Controller::SetFailures(const std::vector<ControllerFailure>& f)
{
	failures = f;
}

const std::vector<ControllerFailure>& Controller::GetFailures() const
{
	return failures;
}

void Controller::Run(Control& u, const Maneuver& man, const double& time)
{
	currentTime = time;
	
	ComputeControl(u, man);
	
	//WARNING There is no guarantee on which failure runs first. So 
	// when writing your failures you should be careful that the effect
	// is independent from the ordering
	for (auto itr = failures.begin(); itr != failures.end(); itr++)
		itr->Run(u, man, time);
}


