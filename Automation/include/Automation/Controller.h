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
	TimedContainer<Maneuver> selfManeuvers;
public:
	virtual Control ComputeControl() const = 0;
	
	// By default, stores only last sensor data. If a different behaviour
	// is desired, please override this function in your controller
	virtual void ReceiveSensorOutput(const SensorOutput&, const double& currentTime);
	
	
	
};

#endif
