#ifndef OBSERVER_H
#define OBSERVER_H

#include "Automation/SensorOutput.h"
#include "Utility/TimedContainer.h"

/**
 * @brief This is abstract base class Observer
 *
 */
class Observer
{
		
	// The observer can store trajectories in memory
	// and use them to determine next maneuver
	TimedContainer<Agent> selfTrajectory;
	TimedContainer<AgentVector> othersTrajectory;
	TimedContainer<EnvironmentParameters> environmentTrajectory;
	
	
	
public:
	virtual ~Observer();
	
	virtual void ReceiveSensorOutput(const SensorOutput&, const double& currentTime);
	virtual void Run();
};

#endif
