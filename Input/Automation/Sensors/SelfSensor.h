#ifndef SELF_SENSOR_H
#define SELF_SENSOR_H

#include "Automation/InternalSensor.h"
#include <set>

class SelfSensor : public InternalSensor
{
public:
	SelfSensor();
	void SimulateOutput(InternalSensorOutput& outputInWorld, const Agent& selfInWorld) const override;
};

#endif
