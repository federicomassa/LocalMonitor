#ifndef POWNET_EXTERNAL_SENSOR_H
#define POWNET_EXTERNAL_SENSOR_H

#include "Automation/ExternalSensor.h"
#include <set>

class PowNetExternalSensor : public ExternalSensor
{
public:
	PowNetExternalSensor();
	void SimulateVisibility(StateRegion& visible, std::set<std::string>& visibleIDs, const Agent& selfInWorld,
						   const AgentVector& othersInWorld) const override;
						   
	void SimulateOutput(ExternalSensorOutput& outputInWorld, const Agent& selfInWorld, const AgentVector& othersInWorld, const EnvironmentParameters& envParam) const override;
};

#endif
