#ifndef POWNET_INTERNAL_SENSOR_H
#define POWNET_INTERNAL_SENSOR_H

#include "Automation/InternalSensor.h"
#include <set>

class PowNetInternalSensor : public InternalSensor
{
public:
	PowNetInternalSensor();
	void SimulateOutput(InternalSensorOutput& outputInWorld, const Agent& selfInWorld) const override;
};

#endif
