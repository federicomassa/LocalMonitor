#ifndef OBSERVER_H
#define OBSERVER_H

#include "Automation/SensorOutput.h"
#include "Utility/TimedContainer.h"
#include "json.hpp"

#include <string>

/**
 * @brief This is abstract base class Observer
 *
 */
class Observer
{
	std::string name;
	std::string observedID;
	
protected:
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
	const std::string& GetObservedID() const;
	void SetClassName(const std::string&);
	void SetObservedID(const std::string&);
	
	// This method defines how the observer stores sensor data into memory. By default, it only stores latest sensor data
	virtual void ReceiveSensorOutput(const SensorOutput&, const double& currentTime);
	
	// This is the main method. It defines what happens each time the observer is run.
	virtual void Run(const double& currentTime) = 0;
	
	// This is useful to customize observer configuration file for each type of observer. By default, only "observing" entry is mandatory and inside it only "id" and "type" entries are mandatory. With this, you can add more.
	virtual void Configure(const nlohmann::json&);
};

#endif
