#include "Sensors.h"
#include "VisionSensor.h"
#include "SelfSensor.h"
#include "PowNetInternalSensor.h"
#include "PowNetExternalSensor.h"

InternalSensor* InstantiateInternalSensor(const std::string& sensorClassName) 
{
	if (sensorClassName == "SelfSensor")
		return new SelfSensor;
	else if (sensorClassName == "PowNetInternalSensor")
		return new PowNetInternalSensor;
	else
	{
		std::cerr << "Error in configure: " << sensorClassName << ".h not found";
		exit(1);
	}
}

ExternalSensor* InstantiateExternalSensor(const std::string& sensorClassName) 
{
	if (sensorClassName == "VisionSensor")
		return new VisionSensor;
	else if (sensorClassName == "PowNetExternalSensor")
		return new PowNetExternalSensor;
	else 
		{
			std::cerr << "Error in configure: " << sensorClassName << ".h not found";
			exit(1);
		} 
}
