// Controller object, keeps memory of everything that is measured (optionally with a threshold) and 

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Utility/TimedContainer.h"
#include "Utility/EnvironmentParameters.h"
#include "Basic/Agent.h"
#include "Automation/Control.h"
#include "Automation/SensorOutput.h"
#include "Basic/Maneuver.h"

// This is the base class for controllers. Every controller **can (it must be specified in the children)** if and how it should store in memory the trajectories as seen by sensors. This info can be used to plan the controller.
class Controller
{
	TimedContainer<Agent> selfTrajectory;
	TimedContainer<AgentVector> othersTrajectory;
	TimedContainer<EnvironmentParameters> environmentTrajectory;
public:
	virtual void ComputeControl(Control&, const Maneuver&) const = 0;
	
	// By default, stores only last sensor data. If a different behaviour
	// is desired, please override this function in your controller
	// TODO make other functions to facilitate this task for the user
	virtual void ReceiveSensorOutput(const SensorOutput&, const double& currentTime);

	const TimedContainer<Agent>& GetSelfTrajectory() const;
	const TimedContainer<AgentVector>& GetOtherAgentsTrajectory() const;
	const TimedContainer<EnvironmentParameters>& GetEnvironmentTrajectory() const;
	
	virtual ~Controller();
	
};

#endif
