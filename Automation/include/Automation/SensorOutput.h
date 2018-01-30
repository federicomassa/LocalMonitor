#ifndef SENSOR_OUTPUT_H
#define SENSOR_OUTPUT_H

#include "Basic/Agent.h"
#include "Utility/EnvironmentParameters.h"
#include "Utility/WorldFeatures.h"


class ExternalSensorOutput;
class InternalSensorOutput;

// This object merges (possibly filters) all the sensor information at a given time instant. 
// NB measures might not contain every world_agent/environment_feature,
// it depends on what sensors the agent use and on their visibility range
class SensorOutput
{
	AgentVector measuredAgents;
	Agent selfMeasure;
	EnvironmentParameters measuredEnvParams;
	
	// Counts how many info there are on each variable
	std::map<std::string, int> externalSensorsCounter;
public:
	SensorOutput();
	
	// Merge information of external sensor
	void MergeExternalSensor(const ExternalSensorOutput&);
	// Merge information of internal sensor
	void MergeInternalSensor(const InternalSensorOutput&);
	
	// Return total sensory system output
	void RetrieveSensorData(Agent& self, AgentVector& others, EnvironmentParameters& env);
	
	const Agent& RetrieveSelfData() const;
	const AgentVector& RetrieveOtherAgentsData() const;
	const EnvironmentParameters& RetrieveEnvironmentData() const;
	
	void SetSelf(const Agent&);
	void SetOthers(const AgentVector&);
	void SetEnvironment(const EnvironmentParameters&);
	
	
};

#endif
