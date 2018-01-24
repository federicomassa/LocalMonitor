#ifndef NAIVE_ENVIRONMENT_H
#define NAIVE_ENVIRONMENT_H

// This object contains a complete prediction, 

#include "Basic/Agent.h"
#include "Utility/EnvironmentParameters.h"
#include "Automation/SensorOutput.h"
#include <memory>

class ExternalSensor;
class InternalSensor;

class NaiveEnvironment
{
	const std::vector<std::shared_ptr<ExternalSensor> >& extSensors;
	const std::vector<std::shared_ptr<InternalSensor> >& intSensors;
	
	Agent self;
	AgentVector others;
	EnvironmentParameters env;
	bool hasHiddenAgent;
	
	// This methods are used to simulate sensors of observed agent
	SensorOutput SimulateSensors(const Agent& trueSelfInWorld, const AgentVector& trueOthersInWorld, const EnvironmentParameters& trueEnvParams);
	
	ExternalSensorOutput RetrieveExternalSensorOutput(const std::string& sensorName, const Agent& trueSelfInWorld, const AgentVector& trueOthersInWorld, const EnvironmentParameters& trueEnvParams);
	
	InternalSensorOutput RetrieveInternalSensorOutput(const std::string& sensorName, const Agent& trueSelfInWorld);
	
public:
	NaiveEnvironment(const Agent&, const AgentVector&, const EnvironmentParameters&,
		const std::vector<std::shared_ptr<ExternalSensor> >&, const std::vector<std::shared_ptr<InternalSensor> >&, const bool& hidden = true
	);
	
	
	
};

#endif
