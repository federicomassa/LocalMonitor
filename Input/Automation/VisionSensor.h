#ifndef VISION_SENSOR_H
#define VISION_SENSOR_H

#include "Automation/ExternalSensor.h"
#include <set>

class VisionSensor : public ExternalSensor
{
public:
	VisionSensor();
	void SimulateVisibility(StateRegion& visible, std::set<std::string>& visibleIDs, const Agent& selfInWorld,
						   const AgentVector& othersInWorld) const override;
						   
	void SimulateOutput(SensorOutput& outputInWorld, const Agent& selfInWorld, const AgentVector& othersInWorld, const SimulationParameters& envParam) const override;
};

#endif
