#ifndef OBSERVER_H
#define OBSERVER_H

#include "Automation/SensorOutput.h"
#include "Utility/TimedContainer.h"

#include <string>

/**
 * @brief This is abstract base class Observer
 *
 */
class Observer
{
	std::string name;
	// The observer can store trajectories in memory
	// and use them to determine next maneuver
	TimedContainer<Agent> selfTrajectory;
	TimedContainer<AgentVector> othersTrajectory;
	TimedContainer<EnvironmentParameters> environmentTrajectory;
	
	
public:
	virtual ~Observer();
	Observer(const std::string& name);
	Observer(const Observer&);
	Observer& operator=(const Observer&);
	bool operator==(const Observer&) const;
	bool operator!=(const Observer&) const;
	const std::string& GetName() const;
	virtual void ReceiveSensorOutput(const SensorOutput&, const double& currentTime);
	virtual void Run() = 0;
};

#endif
