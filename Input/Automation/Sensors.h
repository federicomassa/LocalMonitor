#ifndef SENSORS_H
#define SENSORS_H

#include <string>
#include <iostream>

#include "Automation/InternalSensor.h"
#include "Automation/ExternalSensor.h"
#include "VisionSensor.h"
#include "VelocitySensor.h"
#include "SelfSensor.h"

InternalSensor* InstantiateInternalSensor(const std::string& sensorClassName) 
{
	if (sensorClassName == "VisionSensor")
		return new VisionSensor;
	else if (sensorClassName == "VelocitySensor")
		return new VelocitySensor;
	else if (sensorClassName == "SelfSensor")
		return new SelfSensor;
	else 
		{
			std::cerr << "Error in configure: " << sensorClassName << ".h not found";
			exit(1);
		} 
}

ExternalSensor* InstantiateExternalSensor(const std::string& sensorClassName) 
{
	std::cerr << "Error in configure: " << sensorClassName << ".h not found";
	exit(1);
}

#endif
